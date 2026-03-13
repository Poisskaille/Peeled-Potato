#pragma once

#include <Termina/Renderer/RenderPass.hpp>

namespace Termina {

    /// Compute lighting pass: reads GBuffer, outputs HDR color via UAV.
    class DeferredPass : public RenderPass
    {
    public:
        DeferredPass();
        ~DeferredPass() override;

        void Resize(int32 width, int32 height) override;
        void Execute(RenderPassExecuteInfo& info) override;

    private:
        RendererTexture* m_HDRTexture = nullptr; // RGBA16_FLOAT
    };

} // namespace Termina
