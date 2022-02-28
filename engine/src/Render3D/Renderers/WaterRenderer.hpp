#pragma once

#include "Camera.hpp"
#include "DeferredRenderer.hpp"
#include "Framebuffer.hpp"
#include "GRenderer.hpp"
#include "ModelManager.hpp"
#include "ModelRenderer.hpp"
#include "RenderSettings.hpp"
#include "RendererState.hpp"
#include "Scene.hpp"
#include "Shader.hpp"
#include "SkyboxRenderer.hpp"
#include "Texture.hpp"
#include "Viewport.hpp"

#include <memory>

namespace Engine {

class WaterRenderer {
  private:
    Shader m_Shader;
    unsigned int m_WaterVAO, m_WaterVBO;
    Texture m_WaterDudvMap;
    Texture m_WaterNormalMap;
    float m_WaterMoveFactor = 0.0f;

    Framebuffer m_ReflectionFbo;
    Texture m_ReflectionColorAttachment, m_ReflectionDepthAttachment;
    const unsigned int c_ReflectionWidth = 320, c_ReflectionHeight = 180;

    Framebuffer m_RefractionFbo;
    Texture m_RefractionColorAttachment;
    const unsigned int c_RefractionWidth = 1280, c_RefractionHeight = 720;

    Texture m_PositionAttachment, m_NormalAttachment, m_DepthAttachment, m_SpecularAttachment;

    Viewport &m_Viewport;
    GRenderer &m_GRenderer;
    DeferredRenderer &m_DeferredRenderer;

  public:
    WaterRenderer(Viewport &viewport, GRenderer &gRenderer, DeferredRenderer &deferredRenderer);
    ~WaterRenderer();

    void draw(Camera &camera, Scene &scene, const ModelManager &models, RendererState &state, RenderSettings &settings);
};

} // namespace Engine