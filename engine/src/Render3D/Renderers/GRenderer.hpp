#pragma once

#include "Camera.hpp"
#include "Framebuffer.hpp"
#include "ModelManager.hpp"
#include "ModelRenderer.hpp"
#include "ParticlesRenderer.hpp"
#include "RenderSettings.hpp"
#include "RendererState.hpp"
#include "Scene.hpp"
#include "Shader.hpp"
#include "SkyboxRenderer.hpp"

#include <glm/mat4x4.hpp>

namespace Engine {

class GRenderer {
  private:
    Shader m_Shader;

    ModelRenderer &m_ModelRenderer;
    SkyboxRenderer &m_SkyboxRenderer;
    ParticlesRenderer &m_ParticlesRenderer;

    glm::mat4 m_PrevViewProjectionMatrix;

  public:
    GRenderer(ModelRenderer &modelRenderer, SkyboxRenderer &skyboxRenderer, ParticlesRenderer &m_ParticlesRenderer);
    ~GRenderer();

    void draw(Camera &camera, Scene &scene, const ModelManager &models, RenderSettings &settings);
};

} // namespace Engine