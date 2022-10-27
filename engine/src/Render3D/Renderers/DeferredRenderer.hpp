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
#include "Material.hpp"

namespace Engine {

class DeferredRenderer {
  private:
    Shader m_Shader;
    Material m_Material;

    QuadRenderer &m_QuadRenderer;
    DirectedLightRenderer &m_DirectedLightRenderer;

  public:
    DeferredRenderer(DirectedLightRenderer &directedLightRenderer, QuadRenderer &quadRenderer);
    ~DeferredRenderer();

    void draw(Texture &colorMap, Texture &positionMap, Texture &normalMap, Texture &specularMap, Camera &camera,
              Scene &scene, const ModelManager &models, RenderSettings &settings, RendererState &state);

    void draw(Texture &colorMap, Texture &positionMap, Texture &normalMap, Texture &specularMap, Texture &ssaoMap,
              Camera &camera, Scene &scene, const ModelManager &models, RenderSettings &settings, RendererState &state);

  private:
    void doDraw(Texture &colorMap, Texture &positionMap, Texture &normalMap, Texture &specularMap,
                Camera &camera, Scene &scene, const ModelManager &models, RenderSettings &settings,
                RendererState &state);
};

} // namespace Engine