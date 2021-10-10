#pragma once

#include "Camera.hpp"
#include "DirectedLight.hpp"
#include "ModelManager.hpp"
#include "ModelRenderer.hpp"
#include "RendererState.hpp"
#include "Scene.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Viewport.hpp"

#include <memory>

namespace Engine {

class DirectedLightRenderer {
  private:
    std::unique_ptr<Shader> m_DepthShader;
    unsigned int m_DepthMapFBO;
    std::unique_ptr<Texture> m_DepthMap;

    Viewport &m_ViewPort;
    ModelRenderer &m_ModelRenderer;

  public:
    DirectedLightRenderer(Viewport &viewport, ModelRenderer &modelRender);

    void apply(Camera &camera, const DirectedLight &light, Shader &shader, Scene &scene, const ModelManager &models,
               RendererState &state);
    void resize();

    const std::unique_ptr<Texture> &depthMap() { return m_DepthMap; }
};

} // namespace Engine