#pragma once

#include "Camera.hpp"
#include "DirectedLightRenderer.hpp"
#include "Framebuffer.hpp"
#include "QuadRenderer.hpp"
#include "RenderSettings.hpp"
#include "RendererState.hpp"
#include "Scene.hpp"
#include "Shader.hpp"
#include "Viewport.hpp"

namespace Engine {

class DeferredRenderer {
  private:
    Shader m_Shader;

    QuadRenderer &m_QuadRenderer;
    DirectedLightRenderer &m_DirectedLightRenderer;

  public:
    DeferredRenderer(DirectedLightRenderer &directedLightRenderer, QuadRenderer &quadRenderer);

    void draw(Framebuffer &framebuffer, Camera &camera, Scene &scene, const ModelManager &models,
              RenderSettings &settings, RendererState &state);
};

} // namespace Engine