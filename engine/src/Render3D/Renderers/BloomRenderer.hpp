#pragma once

#include "QuadRenderer.hpp"
#include "Texture.hpp"
#include "Framebuffer.hpp"
#include "RenderSettings.hpp"
#include "Shader.hpp"
#include "Viewport.hpp"

namespace Engine {

class BloomRenderer {
  private:
    Shader m_BloomShader, m_BlurShader, m_BlitShader;

    Viewport &m_Viewport;
    QuadRenderer &m_QuadRenderer;

    unsigned int m_BloomScale = 4;

    static constexpr int MIP_COUNT = 7;
    Framebuffer m_MipFramebuffer[MIP_COUNT][2];
    Texture m_MipColorBuffer[MIP_COUNT][2];

  public:
    BloomRenderer(Viewport &viewport, QuadRenderer &quadRenderer);
    ~BloomRenderer();

    void draw(RenderSettings &settings, Texture inputColorBuffer, Texture exposureColorBuffer, Framebuffer outputFramebuffer);
    void resize();
};

} // namespace Engine