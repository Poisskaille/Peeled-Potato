#include "DeferredPass.hpp"

#include <Termina/Core/Application.hpp>
#include <Termina/Renderer/Renderer.hpp>
#include <Termina/Shader/ShaderManager.hpp>
#include <Termina/Shader/ShaderServer.hpp>
#include "RHI/TextureView.hpp"

namespace Termina {

    DeferredPass::DeferredPass()
    {
        RendererDevice* device = Application::GetSystem<RendererSystem>()->GetDevice();
        uint32 width  = Application::Get().GetWindow()->GetPixelWidth();
        uint32 height = Application::Get().GetWindow()->GetPixelHeight();

        m_HDRTexture = device->CreateTexture(TextureDesc()
            .SetSize(width, height)
            .SetFormat(TextureFormat::RGBA16_FLOAT)
            .SetUsage(TextureUsage::SHADER_READ | TextureUsage::SHADER_WRITE));
        m_HDRTexture->SetName("HDR Color");

        ShaderServer& server = Application::GetSystem<ShaderManager>()->GetShaderServer();
        server.WatchPipeline("Assets/Shaders/Deferred.hlsl", PipelineType::Compute);
    }

    DeferredPass::~DeferredPass()
    {
        delete m_HDRTexture;
    }

    void DeferredPass::Resize(int32 width, int32 height)
    {
        m_HDRTexture->Resize(width, height);
    }

    void DeferredPass::Execute(RenderPassExecuteInfo& info)
    {
        ShaderServer& server = Application::GetSystem<ShaderManager>()->GetShaderServer();

        auto getSRV = [&](RendererTexture* tex) {
            return info.ViewCache->GetTextureView(
                TextureViewDesc::CreateDefault(tex, TextureViewType::SHADER_READ, TextureViewDimension::TEXTURE_2D));
        };

        TextureView* hdrUAV = info.ViewCache->GetTextureView(
            TextureViewDesc::CreateDefault(m_HDRTexture, TextureViewType::SHADER_WRITE, TextureViewDimension::TEXTURE_2D));

        struct DeferredPushConstants
        {
            int32 AlbedoIndex;
            int32 NormalsIndex;
            int32 ORMIndex;
            int32 EmissiveIndex;
            int32 OutputIndex;
            int32 Width;
            int32 Height;
        };

        DeferredPushConstants pc;
        pc.AlbedoIndex   = getSRV(info.IO->GetTexture("GBuffer_Albedo"))->GetBindlessIndex();
        pc.NormalsIndex  = getSRV(info.IO->GetTexture("GBuffer_Normals"))->GetBindlessIndex();
        pc.ORMIndex      = getSRV(info.IO->GetTexture("GBuffer_ORM"))->GetBindlessIndex();
        pc.EmissiveIndex = getSRV(info.IO->GetTexture("GBuffer_Emissive"))->GetBindlessIndex();
        pc.OutputIndex   = hdrUAV->GetBindlessIndex();
        pc.Width         = info.Width;
        pc.Height        = info.Height;

        ComputeEncoder* ce = info.Ctx->CreateComputeEncoder("Deferred Pass");
        ce->SetPipeline(server.GetComputePipeline("Assets/Shaders/Deferred.hlsl"));
        ce->SetConstants(sizeof(pc), &pc);
        ce->Dispatch(info.Width, info.Height, 1, 8, 8, 1);
        ce->End();

        info.IO->RegisterTexture("HDRColor", m_HDRTexture);

        info.Ctx->Barrier(TextureBarrier()
            .SetTargetTexture(m_HDRTexture)
            .SetNewLayout(TextureLayout::READ_ONLY)
            .SetDstAccess(ResourceAccess::SHADER_READ)
            .SetDstStage(PipelineStage::COMPUTE_SHADER));
    }

} // namespace Termina
