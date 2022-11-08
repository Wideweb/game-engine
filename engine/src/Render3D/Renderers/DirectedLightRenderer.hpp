#pragma once

#include "Camera.hpp"
#include "Framebuffer.hpp"
#include "ModelManager.hpp"
#include "ModelRenderer.hpp"
#include "RendererState.hpp"
#include "Scene.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Viewport.hpp"
#include "RenderContext.hpp"

namespace Engine {

class DirectedLightRenderer {
  private:
    Shader m_DepthShader;
    Framebuffer m_DepthMapFramebuffer;
    Texture m_DepthMap;

    Viewport &m_Viewport;
    ModelRenderer &m_ModelRenderer;

  public:
    DirectedLightRenderer(Viewport &viewport, ModelRenderer &modelRender);
    ~DirectedLightRenderer();

    void prepareContext(RenderContext& context);
    void apply(Camera &camera, Scene &scene, const ModelManager &models, RendererState &state);
    void resize();
};

} // namespace Engine