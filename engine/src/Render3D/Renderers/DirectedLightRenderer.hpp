#pragma once

#include "Camera.hpp"
#include "DirectedLight.hpp"
#include "Framebuffer.hpp"
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
    Framebuffer m_DepthMapFramebuffer;
    Texture m_DepthMap;

    Viewport &m_Viewport;
    ModelRenderer &m_ModelRenderer;

  public:
    DirectedLightRenderer(Viewport &viewport, ModelRenderer &modelRender);

    void apply(Camera &camera, const DirectedLight &light, Shader &shader, Scene &scene, const ModelManager &models,
               RendererState &state);
    void resize();

    const Texture &depthMap() { return m_DepthMap; }
};

} // namespace Engine