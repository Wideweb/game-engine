#pragma once

#include "Camera.hpp"
#include "Framebuffer.hpp"
#include "Mesh2D.hpp"
#include "ModelManager.hpp"
#include "Renderbuffer.hpp"
#include "Scene.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Viewport.hpp"

#include "DeferredRenderer.hpp"
#include "DirectedLightRenderer.hpp"
#include "FlareRenderer.hpp"
#include "GRenderer.hpp"
#include "ModelRenderer.hpp"
#include "OverlayRenderer.hpp"
#include "ParticlesRenderer.hpp"
#include "QuadRenderer.hpp"
#include "RenderSettings.hpp"
#include "Renderer2D.hpp"
#include "RendererState.hpp"
#include "SkyboxRenderer.hpp"
#include "SpotLightRenderer.hpp"
#include "WaterRenderer.hpp"

#include <glm/mat4x4.hpp>
#include <memory>

namespace Engine {

class MasterRenderer {
  private:
    unsigned int m_BloomScale = 4;

    Shader m_Shader, m_HdrShader, m_BlurShader;

    std::unique_ptr<QuadRenderer> m_QuadRenderer;
    std::unique_ptr<ModelRenderer> m_ModelRenderer;
    std::unique_ptr<OverlayRenderer> m_OverlayRenderer;
    std::unique_ptr<SkyboxRenderer> m_SkyboxRenderer;
    std::unique_ptr<DirectedLightRenderer> m_DirectedLightRenderer;
    std::unique_ptr<SpotLightRenderer> m_SpotLightRenderer;
    std::unique_ptr<WaterRenderer> m_WaterRenderer;
    std::unique_ptr<FlareRenderer> m_FlareRenderer;
    std::unique_ptr<GRenderer> m_GRenderer;
    std::unique_ptr<ParticlesRenderer> m_ParticlesRenderer;
    std::unique_ptr<Renderer2D> m_Renderer2D;
    std::unique_ptr<DeferredRenderer> m_DeferredRenderer;

    Viewport m_Viewport;
    RendererState m_State;

    Texture m_ColorBuffer[2], m_EntityBuffer;
    Renderbuffer m_DepthRenderbuffer;

    Framebuffer m_Framebuffer, m_HdrFramebuffer;

    Framebuffer m_PingpongFramebuffer[2];
    Texture m_PingpongColorBuffer[2];

  public:
    MasterRenderer(unsigned int width, unsigned int height);

    void draw(Camera &camera, Scene &scene, const ModelManager &models, RenderSettings settings);
    void setClearColor(float r, float g, float b, float a);
    void setViewport(int width, int height);
    const Viewport &getViewport();
    void setFramebuffer(Framebuffer &framebuffer);
    Framebuffer &getFramebuffer() { return m_Framebuffer; }
    void clear();

    void drawQuad();

    void draw2D(const std::vector<Mesh2D::Vertex> &vertices, std::vector<uint32_t> &indices, const Texture *texture,
                const glm::mat4 &model);

    const std::unique_ptr<DirectedLightRenderer> &directedLightRenderer() { return m_DirectedLightRenderer; }

  private:
    void updateBloom();
};

} // namespace Engine