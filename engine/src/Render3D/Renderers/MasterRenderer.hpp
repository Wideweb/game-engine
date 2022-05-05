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
#include "FontRenderer.hpp"
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
#include <glm/vec4.hpp>
#include <memory>

namespace Engine {

class MasterRenderer {
  private:
    unsigned int m_BloomScale = 4;

    Shader m_Shader, m_HdrShader, m_BlurGaussianShader, m_SSAOShader, m_BlurSimpleShader, m_GammaShader;

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
    std::unique_ptr<FontRenderer> m_FontRenderer;

    Viewport m_Viewport;
    RendererState m_State;

    Texture m_ColorBuffer[2], m_EntityBuffer, m_TmpColorBuffer, m_TmpEntityBuffer;
    Renderbuffer m_DepthRenderbuffer;

    Framebuffer m_Framebuffer, m_TmpFramebuffer, m_HdrFramebuffer, m_OutFramebuffer;

    Framebuffer m_PingpongFramebuffer[2];
    Texture m_PingpongColorBuffer[2];

    Framebuffer m_SSAOFramebuffer;
    Texture m_SSAONoiseTexture, m_SSAOBuffer;

    Texture m_GColorAttachment, m_GPositionAttachment, m_GNormalAttachment, m_GSpecularAttachment;
    Framebuffer m_GFramebuffer;

    Texture m_BlurAttachment;
    Framebuffer m_BlurFramebuffer;

    glm::vec4 m_ClearColor = glm::vec4(0.0f);

  public:
    MasterRenderer(unsigned int width, unsigned int height);
    ~MasterRenderer();

    void draw(Camera &camera, Scene &scene, const ModelManager &models, RenderSettings settings);
    void setClearColor(glm::vec4 color);
    glm::vec4 getClearColor();
    void setViewport(int width, int height);
    const Viewport &getViewport();
    void setFramebuffer(Framebuffer &framebuffer);
    Framebuffer &getFramebuffer() { return m_Framebuffer; }
    void clear();

    void drawQuad();

    void draw2D(Texture &texture, glm::vec2 position, glm::vec2 size, glm::vec4 color = glm::vec4(1.0f),
                uint32_t id = 0);
    void draw2D(const std::vector<Mesh2D::Vertex> &vertices, std::vector<uint32_t> &indices, Texture &texture,
                const glm::mat4 &model, uint32_t id = 0);

    const std::unique_ptr<DirectedLightRenderer> &directedLightRenderer() { return m_DirectedLightRenderer; }

  private:
    void updateBloom();
};

} // namespace Engine