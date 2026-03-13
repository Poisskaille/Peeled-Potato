#pragma once

#include <Termina/Renderer/RenderPass.hpp>
#include <Termina/RHI/Sampler.hpp>

namespace Termina {

    /// Geometry pass: renders all mesh actors into the GBuffer MRTs.
    class GBufferPass : public RenderPass
    {
    public:
        GBufferPass();
        ~GBufferPass() override;

        void Resize(int32 width, int32 height) override;
        void Execute(RenderPassExecuteInfo& info) override;

    private:
        RendererTexture* m_AlbedoTexture       = nullptr; // RGBA8_UNORM
        RendererTexture* m_NormalsTexture      = nullptr; // RGBA16_FLOAT
        RendererTexture* m_ORMTexture          = nullptr; // RGBA8_UNORM
        RendererTexture* m_EmissiveTexture     = nullptr; // RGBA16_FLOAT
        RendererTexture* m_MotionVecTexture    = nullptr; // RGBA16_FLOAT
        RendererTexture* m_DepthTexture        = nullptr; // D32_FLOAT
        Sampler*         m_Sampler             = nullptr;
    };

} // namespace Termina
