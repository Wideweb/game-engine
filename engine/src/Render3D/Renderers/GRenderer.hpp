#pragma once

#include "Camera.hpp"
#include "Framebuffer.hpp"
#include "ModelManager.hpp"
#include "ModelRenderer.hpp"
#include "RenderSettings.hpp"
#include "RendererState.hpp"
#include "Scene.hpp"
#include "Shader.hpp"
#include "SkyboxRenderer.hpp"

namespace Engine {

class GRenderer {
  private:
    Shader m_Shader;

    ModelRenderer &m_ModelRenderer;
    SkyboxRenderer &m_SkyboxRenderer;

  public:
    GRenderer(ModelRenderer &modelRenderer, SkyboxRenderer &skyboxRenderer);

    void draw(Camera &camera, Scene &scene, const ModelManager &models, RenderSettings &settings);
};

} // namespace Engine