#pragma once

#include <Termina/Renderer/RenderPass.hpp>

namespace Termina {

    /// Compute pass: ACES filmic tonemap from HDRColor → LDR RendererOutput.
    class TonemapPass : public RenderPass
    {
    public:
        TonemapPass();
        ~TonemapPass() override;

        void Resize(int32 width, int32 height) override;
        void Execute(RenderPassExecuteInfo& info) override;

    private:
        RendererTexture* m_LDRTexture = nullptr; // RGBA8_UNORM
    };

} // namespace Termina
