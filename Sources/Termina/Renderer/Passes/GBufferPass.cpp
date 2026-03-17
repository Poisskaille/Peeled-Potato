#include "GBufferPass.hpp"

#include <Termina/Core/Application.hpp>
#include <Termina/Asset/AssetSystem.hpp>
#include <Termina/Asset/Model/ModelAsset.hpp>
#include <Termina/Asset/Material/MaterialAsset.hpp>
#include <Termina/Renderer/Components/MeshComponent.hpp>
#include <Termina/Renderer/Renderer.hpp>
#include <Termina/Renderer/GPUInstance.hpp>
#include <Termina/Renderer/Material.hpp>
#include <Termina/Shader/ShaderManager.hpp>
#include <Termina/Shader/ShaderServer.hpp>
#include "World/Components/Transform.hpp"
#include "RHI/TextureView.hpp"
#include "Renderer/Passes/DebugPass.hpp"
#include "ImGui/imgui.h"

#include <GLM/glm.hpp>

#include <unordered_map>
#include <cstring>

namespace {

    // Extract 6 frustum planes from a view-projection matrix.
    // Plane equation: dot(plane.xyz, p) + plane.w >= 0 means inside.
    // Uses Gribb-Hartmann method; assumes Vulkan NDC (z in [0,1]).
    void ExtractFrustumPlanes(const glm::mat4& vp, glm::vec4 planes[6])
    {
        // GLM is column-major: vp[col][row]
        auto row = [&](int i) { return glm::vec4(vp[0][i], vp[1][i], vp[2][i], vp[3][i]); };
        glm::vec4 r0 = row(0), r1 = row(1), r2 = row(2), r3 = row(3);

        planes[0] = r3 + r0; // Left
        planes[1] = r3 - r0; // Right
        planes[2] = r3 + r1; // Bottom
        planes[3] = r3 - r1; // Top
        planes[4] = r2;       // Near  (Vulkan: z >= 0)
        planes[5] = r3 - r2;  // Far   (Vulkan: z <= 1)

        for (int i = 0; i < 6; ++i)
        {
            float len = glm::length(glm::vec3(planes[i]));
            if (len > 1e-6f) planes[i] /= len;
        }
    }

    // Returns true if the world-space AABB intersects or is inside the frustum.
    bool IsAABBVisible(const glm::vec4 planes[6], const glm::vec3& aabbMin, const glm::vec3& aabbMax)
    {
        for (int i = 0; i < 6; ++i)
        {
            const glm::vec4& p = planes[i];
            glm::vec3 pv(
                p.x >= 0.f ? aabbMax.x : aabbMin.x,
                p.y >= 0.f ? aabbMax.y : aabbMin.y,
                p.z >= 0.f ? aabbMax.z : aabbMin.z
            );
            if (p.x * pv.x + p.y * pv.y + p.z * pv.z + p.w < 0.f)
                return false;
        }
        return true;
    }

    // Transform an AABB by a matrix, returning the tightest enclosing world-space AABB.
    Termina::AABB TransformAABB(const Termina::AABB& aabb, const glm::mat4& m)
    {
        Termina::AABB result;
        glm::vec3 corners[8] = {
            { aabb.Min.x, aabb.Min.y, aabb.Min.z },
            { aabb.Max.x, aabb.Min.y, aabb.Min.z },
            { aabb.Min.x, aabb.Max.y, aabb.Min.z },
            { aabb.Max.x, aabb.Max.y, aabb.Min.z },
            { aabb.Min.x, aabb.Min.y, aabb.Max.z },
            { aabb.Max.x, aabb.Min.y, aabb.Max.z },
            { aabb.Min.x, aabb.Max.y, aabb.Max.z },
            { aabb.Max.x, aabb.Max.y, aabb.Max.z },
        };
        for (const auto& c : corners)
        {
            glm::vec4 wc = m * glm::vec4(c, 1.0f);
            result.Expand(glm::vec3(wc));
        }
        return result;
    }

} // anonymous namespace

namespace Termina {

    GBufferPass::GBufferPass()
    {
        RendererDevice* device = Application::GetSystem<RendererSystem>()->GetDevice();
        uint32 width  = Application::Get().GetWindow()->GetPixelWidth();
        uint32 height = Application::Get().GetWindow()->GetPixelHeight();

        auto colorRT = TextureUsage::RENDER_TARGET | TextureUsage::SHADER_READ;
        auto floatRT = TextureUsage::RENDER_TARGET | TextureUsage::SHADER_READ;

        m_AlbedoTexture = device->CreateTexture(TextureDesc()
            .SetSize(width, height)
            .SetFormat(TextureFormat::RGBA8_UNORM)
            .SetUsage(colorRT));
        m_AlbedoTexture->SetName("GBuffer Albedo");

        m_NormalsTexture = device->CreateTexture(TextureDesc()
            .SetSize(width, height)
            .SetFormat(TextureFormat::RGBA16_FLOAT)
            .SetUsage(floatRT));
        m_NormalsTexture->SetName("GBuffer Normals");

        m_ORMTexture = device->CreateTexture(TextureDesc()
            .SetSize(width, height)
            .SetFormat(TextureFormat::RGBA8_UNORM)
            .SetUsage(colorRT));
        m_ORMTexture->SetName("GBuffer ORM");

        m_EmissiveTexture = device->CreateTexture(TextureDesc()
            .SetSize(width, height)
            .SetFormat(TextureFormat::RGBA16_FLOAT)
            .SetUsage(floatRT));
        m_EmissiveTexture->SetName("GBuffer Emissive");

        m_MotionVecTexture = device->CreateTexture(TextureDesc()
            .SetSize(width, height)
            .SetFormat(TextureFormat::RGBA16_FLOAT)
            .SetUsage(floatRT));
        m_MotionVecTexture->SetName("GBuffer MotionVectors");

        m_DepthTexture = device->CreateTexture(TextureDesc()
            .SetSize(width, height)
            .SetFormat(TextureFormat::D32_FLOAT)
            .SetUsage(TextureUsage::DEPTH_TARGET | TextureUsage::SHADER_READ));
        m_DepthTexture->SetName("GBuffer Depth");

        m_Sampler = device->CreateSampler(SamplerDesc()
            .SetFilter(SamplerFilter::LINEAR)
            .SetAddress(SamplerAddressMode::WRAP)
            .EnableMips(true));

        // --- Global scene buffers ---
        // SHADER_READ | TRANSFER → eCpuToGpu: host-visible + GPU-readable as storage buffer.
        // One buffer per frame-in-flight so the CPU can write frame N+1 while the GPU
        // is still reading frame N, avoiding the data race that caused flickering on Windows.
        for (int i = 0; i < FRAMES_IN_FLIGHT; ++i)
        {
            m_InstanceBuffer[i] = device->CreateBuffer(BufferDesc()
                .SetSize(sizeof(GPUInstance) * MAX_INSTANCES)
                .SetStride(sizeof(GPUInstance))
                .SetUsage(BufferUsage::SHADER_READ | BufferUsage::TRANSFER));
            m_InstanceBuffer[i]->SetName("Instance Buffer");
            m_InstanceMapped[i] = m_InstanceBuffer[i]->Map();

            m_InstanceBufView[i] = device->CreateBufferView(BufferViewDesc()
                .SetBuffer(m_InstanceBuffer[i])
                .SetType(BufferViewType::SHADER_READ));

            m_MaterialBuffer[i] = device->CreateBuffer(BufferDesc()
                .SetSize(sizeof(GPUMaterial) * MAX_MATERIALS)
                .SetStride(sizeof(GPUMaterial))
                .SetUsage(BufferUsage::SHADER_READ | BufferUsage::TRANSFER));
            m_MaterialBuffer[i]->SetName("Material Buffer");
            m_MaterialMapped[i] = m_MaterialBuffer[i]->Map();

            m_MaterialBufView[i] = device->CreateBufferView(BufferViewDesc()
                .SetBuffer(m_MaterialBuffer[i])
                .SetType(BufferViewType::SHADER_READ));
        }

        // --- Pipeline ---
        RenderPipelineDesc rpDesc = RenderPipelineDesc()
            .SetName("GBuffer Pipeline")
            .AddColorAttachmentFormat(TextureFormat::RGBA8_UNORM)   // albedo
            .AddColorAttachmentFormat(TextureFormat::RGBA16_FLOAT)  // normals
            .AddColorAttachmentFormat(TextureFormat::RGBA8_UNORM)   // ORM
            .AddColorAttachmentFormat(TextureFormat::RGBA16_FLOAT)  // emissive
            .AddColorAttachmentFormat(TextureFormat::RGBA16_FLOAT)  // motion vectors
            .SetDepthAttachmentFormat(TextureFormat::D32_FLOAT)
#if defined(TRMN_MACOS)
            .SetCullMode(PipelineCullMode::FRONT)
#else
            .SetCullMode(PipelineCullMode::BACK)
#endif
            .SetDepthCompareOp(PipelineCompareOp::LESS)
            .SetDepthReadEnabled(true)
            .SetDepthWriteEnabled(true)
            .SetPushConstantSize(128);

        ShaderServer& server = Application::GetSystem<ShaderManager>()->GetShaderServer();
        server.WatchPipeline("__TERMINA__/CORE_SHADERS/GBuffer.hlsl", rpDesc, PipelineType::Graphics);
    }

    GBufferPass::~GBufferPass()
    {
        for (int i = 0; i < FRAMES_IN_FLIGHT; ++i)
        {
            m_InstanceBuffer[i]->Unmap();
            m_MaterialBuffer[i]->Unmap();

            delete m_InstanceBufView[i];
            delete m_MaterialBufView[i];
            delete m_InstanceBuffer[i];
            delete m_MaterialBuffer[i];
        }

        delete m_Sampler;
        delete m_AlbedoTexture;
        delete m_NormalsTexture;
        delete m_ORMTexture;
        delete m_EmissiveTexture;
        delete m_MotionVecTexture;
        delete m_DepthTexture;
    }

    void GBufferPass::Resize(int32 width, int32 height)
    {
        m_AlbedoTexture->Resize(width, height);
        m_NormalsTexture->Resize(width, height);
        m_ORMTexture->Resize(width, height);
        m_EmissiveTexture->Resize(width, height);
        m_MotionVecTexture->Resize(width, height);
        m_DepthTexture->Resize(width, height);
    }

    void GBufferPass::Execute(RenderPassExecuteInfo& info)
    {
        ShaderServer& server = Application::GetSystem<ShaderManager>()->GetShaderServer();

        // --- Frustum culling setup ---
        // When frozen, use the stored VP for culling; otherwise use the current camera.
        if (!m_FreezeFrustum)
            m_FrustumFrozen = false;

        const glm::mat4& cullVP = m_FrustumFrozen ? m_FrozenViewProj : info.CurrentCamera.ViewProjection;

        // Snapshot the current VP the first frame freeze is activated.
        if (m_FreezeFrustum && !m_FrustumFrozen)
        {
            m_FrozenViewProj = info.CurrentCamera.ViewProjection;
            m_FrustumFrozen  = true;
        }

        glm::vec4 frustumPlanes[6];
        ExtractFrustumPlanes(cullVP, frustumPlanes);

        // --- Build instance and material lists, writing directly into mapped buffers ---
        // Select the slot for this frame so we never overwrite a buffer the GPU is still reading.
        const int frameSlot = static_cast<int>(info.FrameIndex % FRAMES_IN_FLIGHT);
        GPUInstance* instanceDst = static_cast<GPUInstance*>(m_InstanceMapped[frameSlot]);
        GPUMaterial* materialDst = static_cast<GPUMaterial*>(m_MaterialMapped[frameSlot]);

        int32 instanceCount = 0;
        int32 materialCount = 0;
        int32 totalCount    = 0;

        std::unordered_map<MaterialAsset*, int32> materialMap;

        const int32 samplerIndex = m_Sampler->GetBindlessHandle();

        auto resolveTextureIndex = [&](const AssetHandle<TextureAsset>& handle) -> int32 {
            if (!handle.IsValid()) return -1;
            RendererTexture* tex = handle->GetTexture();
            if (!tex) return -1;
            TextureView* tv = info.ViewCache->GetTextureView(
                TextureViewDesc::CreateDefault(tex, TextureViewType::SHADER_READ, TextureViewDimension::TEXTURE_2D));
            return tv->GetBindlessIndex();
        };

        // Collect draw calls while filling scene buffers
        struct DrawCall { int32 instanceID; RendererBuffer* indexBuffer; uint32 indexCount; uint32 indexOffset; AABB worldBounds; };
        std::vector<DrawCall> draws;

        for (auto& actor : info.CurrentWorld->GetActors())
        {
            if (!actor->HasComponent<MeshComponent>()) continue;

            auto& meshComp = actor->GetComponent<MeshComponent>();
            ModelAsset* model = meshComp.Model.Get();
            if (!model || !model->VertexBuffer || !model->IndexBuffer || !model->VertexView)
                continue;

            const int32     vbIndex    = model->VertexView->GetBindlessHandle();
            const glm::mat4 actorWorld = actor->GetComponent<Transform>().GetWorldMatrix();

            for (const MeshInstance& inst : model->Instances)
            {
                if (inst.LODs.empty()) continue;
                if (instanceCount >= MAX_INSTANCES) break;

                ++totalCount;

                // CPU frustum cull: transform the mesh's local AABB into world space and test.
                AABB worldBounds = TransformAABB(inst.Bounds, actorWorld);
                if (!IsAABBVisible(frustumPlanes, worldBounds.Min, worldBounds.Max))
                    continue;

                // Material resolution: per-component override takes priority over the model's own material
                MaterialAsset* matAsset = nullptr;
                {
                    auto overrideIt = meshComp.MaterialOverrides.find(inst.MaterialIndex);
                    if (overrideIt != meshComp.MaterialOverrides.end() && overrideIt->second.IsValid())
                        matAsset = overrideIt->second.Get();
                    else if (inst.MaterialIndex < model->Materials.size())
                        matAsset = model->Materials[inst.MaterialIndex].Get();
                }

                int32 matIdx = 0;
                if (matAsset)
                {
                    auto it = materialMap.find(matAsset);
                    if (it != materialMap.end())
                    {
                        matIdx = it->second;
                    }
                    else if (materialCount < MAX_MATERIALS)
                    {
                        matIdx = materialCount;
                        materialMap[matAsset] = matIdx;

                        const Material& m = matAsset->GetMaterial();
                        GPUMaterial& gpuMat  = materialDst[materialCount++];
                        gpuMat.AlbedoIndex   = resolveTextureIndex(m.AlbedoTexture);
                        gpuMat.NormalIndex   = resolveTextureIndex(m.NormalTexture);
                        gpuMat.ORMIndex      = resolveTextureIndex(m.ORMTexture);
                        gpuMat.EmissiveIndex = resolveTextureIndex(m.EmissiveTexture);
                        gpuMat.SamplerIndex  = samplerIndex;
                        gpuMat.Flags         = (m.AlphaTest        ? kMatFlagAlphaTest        : 0)
                                             | (m.OverrideMetallic  ? kMatFlagOverrideMetallic  : 0)
                                             | (m.OverrideRoughness ? kMatFlagOverrideRoughness : 0);
                        gpuMat.MetallicFactor   = m.MetallicFactor;
                        gpuMat.RoughnessFactor  = m.RoughnessFactor;
                        gpuMat.ColorR           = m.Color.r;
                        gpuMat.ColorG           = m.Color.g;
                        gpuMat.ColorB           = m.Color.b;
                        gpuMat.AlphaCutoff      = 0.5f;
                    }
                }

                // Write instance
                const int32 instID      = instanceCount;
                GPUInstance& gpuInst    = instanceDst[instanceCount++];
                gpuInst.WorldMatrix       = actorWorld * inst.LocalTransform;
                gpuInst.MaterialIndex     = matIdx;
                gpuInst.VertexBufferIndex = vbIndex;
                gpuInst.BaseVertex        = static_cast<int32>(inst.BaseVertex);
                gpuInst._pad              = 0;

                const MeshLOD& lod = inst.LODs[0];
                draws.push_back({ instID, model->IndexBuffer, lod.IndexCount, lod.IndexOffset, worldBounds });
            }
        }

        m_LastTotalCount    = totalCount;
        m_LastCulledCount   = totalCount - instanceCount;
        m_LastDrawCallCount = static_cast<int32>(draws.size());
        int32 triCount = 0;
        for (const DrawCall& dc : draws) triCount += static_cast<int32>(dc.indexCount) / 3;
        m_LastTriangleCount = triCount;

        // --- Debug visualization when frustum is frozen ---
        if (m_FrustumFrozen)
        {
            // Draw the frozen frustum in yellow
            DebugPass::DrawFrustum(m_FrozenViewProj, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));

            // Draw world-space AABBs of all unculled instances in green
            for (const DrawCall& dc : draws)
            {
                glm::vec3 center  = dc.worldBounds.Center();
                glm::vec3 extents = dc.worldBounds.Extents();
                DebugPass::DrawBox(center, extents, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
            }
        }

        // --- Render encoder ---
        auto getRTV = [&](RendererTexture* tex) {
            return info.ViewCache->GetTextureView(
                TextureViewDesc().SetTexture(tex).SetType(TextureViewType::RENDER_TARGET));
        };
        TextureView* depthDTV = info.ViewCache->GetTextureView(
            TextureViewDesc().SetTexture(m_DepthTexture).SetType(TextureViewType::DEPTH_TARGET));

        RenderEncoderInfo rei = RenderEncoderInfo()
            .SetName("GBuffer Pass")
            .SetDimensions(info.Width, info.Height)
            .AddColorAttachment(getRTV(m_AlbedoTexture),    true, glm::vec4(0.0f))
            .AddColorAttachment(getRTV(m_NormalsTexture),   true, glm::vec4(0.0f))
            .AddColorAttachment(getRTV(m_ORMTexture),       true, glm::vec4(0.0f))
            .AddColorAttachment(getRTV(m_EmissiveTexture),  true, glm::vec4(0.0f))
            .AddColorAttachment(getRTV(m_MotionVecTexture), true, glm::vec4(0.0f))
            .SetDepthAttachment(depthDTV);

        RenderEncoder* re = info.Ctx->CreateRenderEncoder(rei);
        re->SetPipeline(server.GetPipeline("__TERMINA__/CORE_SHADERS/GBuffer.hlsl"));
        re->SetViewport(0.0f, 0.0f, static_cast<float>(info.Width), static_cast<float>(info.Height));
        re->SetScissorRect(0, 0, info.Width, info.Height);

        struct GBufferPushConstants
        {
            glm::mat4 ViewProjection;       // 64 bytes
            int32     InstanceID;           //  4 bytes
            int32     InstanceBufferIndex;  //  4 bytes
            int32     MaterialBufferIndex;  //  4 bytes
            int32     _pad;                 //  4 bytes
        };                                  // Total: 80 bytes

        const int32 instBufIdx = m_InstanceBufView[frameSlot]->GetBindlessHandle();
        const int32 matBufIdx  = m_MaterialBufView[frameSlot]->GetBindlessHandle();

        for (const DrawCall& dc : draws)
        {
            re->SetIndexBuffer(dc.indexBuffer);

            GBufferPushConstants pc;
            pc.ViewProjection      = info.CurrentCamera.ViewProjection;
            pc.InstanceID          = dc.instanceID;
            pc.InstanceBufferIndex = instBufIdx;
            pc.MaterialBufferIndex = matBufIdx;
            pc._pad                = 0;

            re->SetConstants(sizeof(pc), &pc);
            re->DrawIndexed(dc.indexCount, 1, dc.indexOffset, 0, 0);
        }

        re->End();

        // Register GBuffer outputs and scene buffers via PassIO
        info.IO->RegisterTexture("GBuffer_Albedo",        m_AlbedoTexture);
        info.IO->RegisterTexture("GBuffer_Normals",       m_NormalsTexture);
        info.IO->RegisterTexture("GBuffer_ORM",           m_ORMTexture);
        info.IO->RegisterTexture("GBuffer_Emissive",      m_EmissiveTexture);
        info.IO->RegisterTexture("GBuffer_MotionVectors", m_MotionVecTexture);
        info.IO->RegisterTexture("GBuffer_Depth",         m_DepthTexture);
        info.IO->RegisterBuffer("Scene_InstanceBuffer",   m_InstanceBuffer[frameSlot]);
        info.IO->RegisterBuffer("Scene_MaterialBuffer",   m_MaterialBuffer[frameSlot]);

        // Transition color targets to shader-readable
        auto colorBarrier = [&](RendererTexture* tex) {
            info.Ctx->Barrier(TextureBarrier()
                .SetTargetTexture(tex)
                .SetNewLayout(TextureLayout::READ_ONLY)
                .SetDstAccess(ResourceAccess::SHADER_READ)
                .SetDstStage(PipelineStage::PIXEL_SHADER));
        };
        colorBarrier(m_AlbedoTexture);
        colorBarrier(m_NormalsTexture);
        colorBarrier(m_ORMTexture);
        colorBarrier(m_EmissiveTexture);
        colorBarrier(m_MotionVecTexture);
    }

    void GBufferPass::Inspect()
    {
        ImGui::Text("Instances: %d / %d (culled: %d)",
            m_LastTotalCount - m_LastCulledCount,
            m_LastTotalCount,
            m_LastCulledCount);

        if (ImGui::Checkbox("Freeze Frustum", &m_FreezeFrustum))
        {
            // When re-enabling, let Execute() snapshot the new VP on the next frame.
            if (m_FreezeFrustum)
                m_FrustumFrozen = false;
        }

        if (m_FrustumFrozen)
        {
            ImGui::SameLine();
            ImGui::TextDisabled("(frozen)");
        }
    }

} // namespace Termina
