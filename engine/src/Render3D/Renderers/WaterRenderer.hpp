#pragma once

#include "Camera.hpp"
#include "GRenderer.hpp"
#include "ModelManager.hpp"
#include "ModelRenderer.hpp"
#include "RenderSettings.hpp"
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

    unsigned int m_ReflectionColor;
    const unsigned int c_ReflectionWidth = 320, c_ReflectionHeight = 180;

    unsigned int m_RefractionColor;
    const unsigned int c_RefractionWidth = 1280, c_RefractionHeight = 720;

    Viewport &m_Viewport;
    GRenderer &m_GRenderer;

  public:
    WaterRenderer(Viewport &viewport, GRenderer &gRenderer);

    void draw(Camera &camera, Scene &scene, const ModelManager &models, RendererState &state, RenderSettings &settings);
};

} // namespace Engine