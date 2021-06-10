#pragma once

#include "Camera.hpp"
#include "GRenderer.hpp"
#include "ModelManager.hpp"
#include "ModelRenderer.hpp"
#include "RendererState.hpp"
#include "Scene.hpp"
#include "Shader.hpp"
#include "SkyboxRenderer.hpp"
#include "Texture.hpp"

#include <memory>

namespace Engine {

class WaterRenderer {
  private:
    std::unique_ptr<Shader> m_WaterShader;
    unsigned int m_WaterVAO, m_WaterVBO;
    std::shared_ptr<Texture> m_WaterDudvMap;
    std::shared_ptr<Texture> m_WaterNormalMap;
    float m_WaterMoveFactor = 0.0f;

    GRenderer &m_GRenderer;

  public:
    WaterRenderer(GRenderer &gRenderer);

    void draw(Camera &camera, Scene &scene, const ModelManager &models, RendererState& state);
};

} // namespace Engine