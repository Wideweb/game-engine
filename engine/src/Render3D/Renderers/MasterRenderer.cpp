#include "MasterRenderer.hpp"

#include "File.hpp"
#include "TextureLoader.hpp"

#include "glad/glad.h"

#include <iostream>

namespace Engine {

MasterRenderer::MasterRenderer(int width, int height) : m_Viewport{width, height} {
    auto vertexSrc = File::read("./shaders/direct-vertex-shader.glsl");
    auto fragmentSrc = File::read("./shaders/direct-fragment-shader.glsl");
    m_Shader = std::make_unique<Shader>(vertexSrc, fragmentSrc);

    vertexSrc = File::read("./shaders/hdr-vertex-shader.glsl");
    fragmentSrc = File::read("./shaders/hdr-fragment-shader.glsl");
    m_HdrShader = std::make_unique<Shader>(vertexSrc, fragmentSrc);

    glGenFramebuffers(1, &m_HdrFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_HdrFBO);

    m_ColorBuffer[0].reset(TextureLoader::createRGBA16Buffer(width, height));
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorBuffer[0]->getId(), 0);

    m_EntityBuffer.reset(TextureLoader::createR32IBuffer(width, height));
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_EntityBuffer->getId(), 0);

    m_ColorBuffer[1].reset(TextureLoader::createRGBA16Buffer(width, height));
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_ColorBuffer[1]->getId(), 0);

    unsigned int attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    glDrawBuffers(3, attachments);

    glGenRenderbuffers(1, &m_DepthRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, m_DepthRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthRBO);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    vertexSrc = File::read("./shaders/blur-vertex-shader.glsl");
    fragmentSrc = File::read("./shaders/blur-fragment-shader.glsl");
    m_BlurShader = std::make_unique<Shader>(vertexSrc, fragmentSrc);

    glGenFramebuffers(2, m_PingpongFBO);

    glBindFramebuffer(GL_FRAMEBUFFER, m_PingpongFBO[0]);
    m_PingpongColorBuffer[0].reset(TextureLoader::createRGBA16Buffer(width / m_BloomScale, height / m_BloomScale));
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_PingpongColorBuffer[0]->getId(), 0);

    glBindFramebuffer(GL_FRAMEBUFFER, m_PingpongFBO[1]);
    m_PingpongColorBuffer[1].reset(TextureLoader::createRGBA16Buffer(width / m_BloomScale, height / m_BloomScale));
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_PingpongColorBuffer[1]->getId(), 0);

    m_QuadRenderer = std::make_unique<QuadRenderer>();
    m_ModelRenderer = std::make_unique<ModelRenderer>();
    m_OverlayRenderer = std::make_unique<OverlayRenderer>();
    m_SkyboxRenderer = std::make_unique<SkyboxRenderer>();
    m_GRenderer = std::make_unique<GRenderer>(m_Viewport, *m_ModelRenderer, *m_SkyboxRenderer);
    m_DirectedLightRenderer = std::make_unique<DirectedLightRenderer>(m_Viewport, *m_ModelRenderer);
    m_SpotLightRenderer = std::make_unique<SpotLightRenderer>(m_Viewport, *m_ModelRenderer);
    m_WaterRenderer = std::make_unique<WaterRenderer>(*m_GRenderer);
    m_FlareRenderer = std::make_unique<FlareRenderer>(m_Viewport, *m_QuadRenderer);
    m_ParticlesRenderer = std::make_unique<ParticlesRenderer>();
    m_Renderer2D = std::make_unique<Renderer2D>();
}

void MasterRenderer::draw(Camera &camera, Scene &scene, const ModelManager &models, RenderSettings settings) {
    if (settings.hdr) {
        m_State.fbo = m_HdrFBO;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, m_State.fbo);
    glViewport(0, 0, m_Viewport.width, m_Viewport.height);

    m_Shader->bind();
    m_Shader->setFloat3("u_viewPos", camera.positionVec());
    m_Shader->setMatrix4("u_view", camera.viewMatrix());
    m_Shader->setMatrix4("u_projection", camera.projectionMatrix());

    m_Shader->setFloat("u_threshold", settings.threshold);

    m_Shader->setInt("u_hasDirectedLight", 0);
    m_Shader->setInt("u_spotLightsNumber", 0);

    if (scene.hasDirectedLight()) {
        m_Shader->setInt("u_hasDirectedLight", 1);
        m_DirectedLightRenderer->apply(camera, scene.getDirectedLight(), *m_Shader, scene, models, m_State);
    }

    for (const auto &obj : scene.getSpotLights()) {
        m_SpotLightRenderer->apply(obj.light, obj.position, *m_Shader, scene, models, m_State);
    }

    m_SkyboxRenderer->draw(camera, scene, settings);

    for (const auto &obj : scene.getParticleEmitters()) {
        m_ParticlesRenderer->draw(obj.particles, obj.position, camera, settings);
    }

    m_ModelRenderer->draw(*m_Shader, scene, models, m_State);
    m_WaterRenderer->draw(camera, scene, models, m_State, settings);

    if (settings.hdr) {
        bool horizontal = true, firstIteration = true;
        if (settings.bloom) {
            if (settings.bloomScale != m_BloomScale) {
                m_BloomScale = settings.bloomScale;
                updateBloom();
            }

            glViewport(0, 0, m_Viewport.width / m_BloomScale, m_Viewport.height / m_BloomScale);
            m_BlurShader->bind();
            glActiveTexture(GL_TEXTURE0);
            m_BlurShader->setInt("u_colorBuffer", 0);
            for (unsigned int i = 0; i < settings.blur; i++) {
                glBindFramebuffer(GL_FRAMEBUFFER, m_PingpongFBO[horizontal]);
                m_BlurShader->setInt("u_horizontal", horizontal);
                if (firstIteration) {
                    m_ColorBuffer[1]->bind();
                    firstIteration = false;

                } else {
                    m_PingpongColorBuffer[!horizontal]->bind();
                }
                m_QuadRenderer->draw();
                horizontal = !horizontal;
            }
            glViewport(0, 0, m_Viewport.width, m_Viewport.height);
        }

        m_State.fbo = m_FBO;
        glBindFramebuffer(GL_FRAMEBUFFER, m_State.fbo);

        m_HdrShader->bind();
        glActiveTexture(GL_TEXTURE0);
        m_ColorBuffer[0]->bind();
        m_HdrShader->setInt("u_hdrBuffer", 0);

        glActiveTexture(GL_TEXTURE1);
        m_PingpongColorBuffer[horizontal]->bind();
        m_HdrShader->setInt("u_blurBuffer", 1);

        glActiveTexture(GL_TEXTURE2);
        m_EntityBuffer->bind();
        m_HdrShader->setInt("u_id", 2);

        m_HdrShader->setFloat("u_exposure", settings.exposure);
        m_HdrShader->setFloat("u_gamma", settings.gamma);
        m_HdrShader->setInt("u_toneMapping", static_cast<int>(settings.toneMapping));

        m_QuadRenderer->draw();
    }

    m_OverlayRenderer->draw(camera, scene, models);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void MasterRenderer::setClearColor(float r, float g, float b, float a) { glClearColor(r, g, b, a); }

void MasterRenderer::setViewport(int width, int height) {
    m_Viewport.width = width;
    m_Viewport.height = height;
    m_GRenderer->resize();
    m_DirectedLightRenderer->resize();

    m_ColorBuffer[0]->bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    m_ColorBuffer[0]->unbind();

    m_ColorBuffer[1]->bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    m_ColorBuffer[1]->unbind();

    m_EntityBuffer->bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, NULL);
    m_EntityBuffer->unbind();

    updateBloom();
}

void MasterRenderer::updateBloom() {
    m_PingpongColorBuffer[0]->bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_Viewport.width / m_BloomScale, m_Viewport.height / m_BloomScale, 0,
                 GL_RGBA, GL_FLOAT, NULL);
    m_PingpongColorBuffer[0]->unbind();

    m_PingpongColorBuffer[1]->bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_Viewport.width / m_BloomScale, m_Viewport.height / m_BloomScale, 0,
                 GL_RGBA, GL_FLOAT, NULL);
    m_PingpongColorBuffer[1]->unbind();
}

const Viewport &MasterRenderer::getViewport() { return m_Viewport; }

void MasterRenderer::setFBO(unsigned int fbo) { m_FBO = fbo; }

void MasterRenderer::clear() {
    m_SpotLightRenderer->clear();
    m_State.activeTextures = 0;
    m_State.fbo = m_FBO;

    glBindFramebuffer(GL_FRAMEBUFFER, m_PingpongFBO[0]);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, m_PingpongFBO[1]);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindFramebuffer(GL_FRAMEBUFFER, m_HdrFBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void MasterRenderer::draw2D(const std::vector<Mesh2D::Vertex> &vertices, std::vector<uint32_t> &indices,
                            const Texture *texture, const glm::mat4 &model) {
    m_Renderer2D->draw(vertices, indices, texture, model);
}

} // namespace Engine