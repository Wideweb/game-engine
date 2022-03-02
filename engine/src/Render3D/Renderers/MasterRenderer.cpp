#include "MasterRenderer.hpp"

#include "File.hpp"
#include "TextureLoader.hpp"

#include "glad/glad.h"

#include <iostream>

namespace Engine {

MasterRenderer::MasterRenderer(unsigned int width, unsigned int height)
    : m_Viewport{width, height}, m_Framebuffer(Framebuffer::createDefault()), m_State{.framebuffer = m_Framebuffer} {
    auto vertexSrc = File::read("./shaders/direct-vertex-shader.glsl");
    auto fragmentSrc = File::read("./shaders/direct-fragment-shader.glsl");
    m_Shader = Shader(vertexSrc, fragmentSrc);

    vertexSrc = File::read("./shaders/hdr-vertex-shader.glsl");
    fragmentSrc = File::read("./shaders/hdr-fragment-shader.glsl");
    m_HdrShader = Shader(vertexSrc, fragmentSrc);

    m_HdrFramebuffer = Framebuffer::create();
    m_HdrFramebuffer.bind();

    m_ColorBuffer[0] = Texture::createRGBA16FBuffer(width, height);
    m_EntityBuffer = Texture::createR32IBuffer(width, height);
    m_ColorBuffer[1] = Texture::createRGBA16FBuffer(width, height);

    m_HdrFramebuffer.addAttachment(m_ColorBuffer[0]);
    m_HdrFramebuffer.addAttachment(m_EntityBuffer);
    m_HdrFramebuffer.addAttachment(m_ColorBuffer[1]);

    m_DepthRenderbuffer = Renderbuffer::create(width, height, Renderbuffer::InternalFormat::DEPTH_COMPONENT);
    m_HdrFramebuffer.setDepthAttachment(m_DepthRenderbuffer);
    m_HdrFramebuffer.check();
    m_HdrFramebuffer.unbind();

    vertexSrc = File::read("./shaders/blur-vertex-shader.glsl");
    fragmentSrc = File::read("./shaders/blur-fragment-shader.glsl");
    m_BlurShader = Shader(vertexSrc, fragmentSrc);

    m_PingpongFramebuffer[0] = Framebuffer::create();
    m_PingpongFramebuffer[0].bind();
    m_PingpongColorBuffer[0] = Texture::createRGBA16FBuffer(width / m_BloomScale, height / m_BloomScale);
    m_PingpongFramebuffer[0].addAttachment(m_PingpongColorBuffer[0]);

    m_PingpongFramebuffer[1] = Framebuffer::create();
    m_PingpongFramebuffer[1].bind();
    m_PingpongColorBuffer[1] = Texture::createRGBA16FBuffer(width / m_BloomScale, height / m_BloomScale);
    m_PingpongFramebuffer[1].addAttachment(m_PingpongColorBuffer[1]);
    m_PingpongFramebuffer[1].unbind();

    m_QuadRenderer = std::make_unique<QuadRenderer>();
    m_ModelRenderer = std::make_unique<ModelRenderer>();
    m_OverlayRenderer = std::make_unique<OverlayRenderer>();
    m_SkyboxRenderer = std::make_unique<SkyboxRenderer>();
    m_GRenderer = std::make_unique<GRenderer>(*m_ModelRenderer, *m_SkyboxRenderer);
    m_DirectedLightRenderer = std::make_unique<DirectedLightRenderer>(m_Viewport, *m_ModelRenderer);
    m_DeferredRenderer = std::make_unique<DeferredRenderer>(*m_DirectedLightRenderer, *m_QuadRenderer);
    m_SpotLightRenderer = std::make_unique<SpotLightRenderer>(m_Viewport, *m_ModelRenderer);
    m_WaterRenderer = std::make_unique<WaterRenderer>(m_Viewport, *m_GRenderer, *m_DeferredRenderer);
    m_FlareRenderer = std::make_unique<FlareRenderer>(m_Viewport, *m_QuadRenderer);
    m_ParticlesRenderer = std::make_unique<ParticlesRenderer>();
    m_Renderer2D = std::make_unique<Renderer2D>();
}

MasterRenderer::~MasterRenderer() {
    m_Framebuffer.free();
    m_HdrFramebuffer.free();

    m_PingpongFramebuffer[0].free();
    m_PingpongFramebuffer[1].free();

    m_PingpongColorBuffer[0].free();
    m_PingpongColorBuffer[1].free();

    m_Shader.free();
    m_HdrShader.free();
    m_BlurShader.free();

    m_ColorBuffer[0].free();
    m_ColorBuffer[1].free();
    m_EntityBuffer.free();
    m_DepthRenderbuffer.free();
}

void MasterRenderer::draw(Camera &camera, Scene &scene, const ModelManager &models, RenderSettings settings) {
    if (settings.hdr) {
        m_State.framebuffer = m_HdrFramebuffer;
    }

    m_State.framebuffer.bind();
    m_Viewport.use();

    m_Shader.bind();
    m_Shader.setFloat3("u_viewPos", camera.positionVec());
    m_Shader.setMatrix4("u_view", camera.viewMatrix());
    m_Shader.setMatrix4("u_projection", camera.projectionMatrix());

    m_Shader.setFloat("u_threshold", settings.threshold);

    m_Shader.setInt("u_hasDirectedLight", 0);
    m_Shader.setInt("u_spotLightsNumber", 0);

    if (scene.hasDirectedLight()) {
        m_Shader.setInt("u_hasDirectedLight", 1);
        m_DirectedLightRenderer->apply(camera, scene.getDirectedLight(), m_Shader, scene, models, m_State);
    }

    for (const auto &obj : scene.getSpotLights()) {
        m_SpotLightRenderer->apply(obj.light, obj.position, m_Shader, scene, models, m_State);
    }

    m_SkyboxRenderer->draw(camera, scene, settings);

    for (const auto &obj : scene.getParticleEmitters()) {
        m_ParticlesRenderer->draw(obj.particles, obj.position, camera, settings);
    }

    m_ModelRenderer->draw(m_Shader, scene, models);
    m_WaterRenderer->draw(camera, scene, models, m_State, settings);

    if (settings.hdr) {
        bool horizontal = true, firstIteration = true;
        if (settings.bloom) {
            if (settings.bloomScale != m_BloomScale) {
                m_BloomScale = settings.bloomScale;
                updateBloom();
            }

            unsigned int lastViewportWidth = m_Viewport.width;
            unsigned int lastViewportHeight = m_Viewport.height;

            m_Viewport.resize(m_Viewport.width / m_BloomScale, m_Viewport.height / m_BloomScale);
            m_BlurShader.bind();
            for (unsigned int i = 0; i < settings.blur; i++) {
                m_PingpongFramebuffer[horizontal].bind();
                m_BlurShader.setInt("u_horizontal", horizontal);
                if (firstIteration) {
                    m_BlurShader.setTexture("u_colorBuffer", m_ColorBuffer[1]);
                    firstIteration = false;
                } else {
                    m_BlurShader.setTexture("u_colorBuffer", m_PingpongColorBuffer[!horizontal]);
                }
                m_QuadRenderer->draw();
                horizontal = !horizontal;
            }
            m_Viewport.resize(lastViewportWidth, lastViewportHeight);
        }

        m_State.framebuffer = m_Framebuffer;
        m_State.framebuffer.bind();

        m_HdrShader.bind();
        m_HdrShader.setTexture("u_hdrBuffer", m_ColorBuffer[0]);
        m_HdrShader.setTexture("u_blurBuffer", m_PingpongColorBuffer[horizontal]);
        m_HdrShader.setTexture("u_id", m_EntityBuffer);
        m_HdrShader.setFloat("u_exposure", settings.exposure);
        m_HdrShader.setFloat("u_gamma", settings.gamma);
        m_HdrShader.setInt("u_toneMapping", static_cast<int>(settings.toneMapping));

        m_QuadRenderer->draw();
    }

    m_OverlayRenderer->draw(camera, scene, models);

    m_Framebuffer.unbind();
}

void MasterRenderer::setClearColor(float r, float g, float b, float a) { glClearColor(r, g, b, a); }

void MasterRenderer::setViewport(int width, int height) {
    m_Viewport.width = width;
    m_Viewport.height = height;
    m_DirectedLightRenderer->resize();

    m_ColorBuffer[0].bind();
    m_ColorBuffer[0].resize(width, height);

    m_ColorBuffer[1].bind();
    m_ColorBuffer[1].resize(width, height);

    m_EntityBuffer.bind();
    m_EntityBuffer.resize(width, height);
    m_EntityBuffer.unbind();

    m_DepthRenderbuffer.bind();
    m_DepthRenderbuffer.resize(width, height);
    m_DepthRenderbuffer.unbind();

    updateBloom();
}

void MasterRenderer::updateBloom() {
    m_PingpongColorBuffer[0].bind();
    m_PingpongColorBuffer[0].resize(m_Viewport.width / m_BloomScale, m_Viewport.height / m_BloomScale);

    m_PingpongColorBuffer[1].bind();
    m_PingpongColorBuffer[1].resize(m_Viewport.width / m_BloomScale, m_Viewport.height / m_BloomScale);
    m_PingpongColorBuffer[1].unbind();
}

const Viewport &MasterRenderer::getViewport() { return m_Viewport; }

void MasterRenderer::setFramebuffer(Framebuffer &framebuffer) { m_Framebuffer = framebuffer; }

void MasterRenderer::clear() {
    m_SpotLightRenderer->clear();
    m_State.framebuffer = m_Framebuffer;

    m_PingpongFramebuffer[0].bind();
    m_PingpongFramebuffer[0].clear();
    m_PingpongFramebuffer[1].bind();
    m_PingpongFramebuffer[1].clear();

    m_HdrFramebuffer.bind();
    m_HdrFramebuffer.clear();

    m_Framebuffer.bind();
    m_Framebuffer.clear();
    m_Framebuffer.unbind();
}

void MasterRenderer::draw2D(const std::vector<Mesh2D::Vertex> &vertices, std::vector<uint32_t> &indices,
                            const Texture *texture, const glm::mat4 &model) {
    m_Renderer2D->draw(vertices, indices, texture, model);
}

} // namespace Engine