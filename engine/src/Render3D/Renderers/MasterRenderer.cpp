#include "MasterRenderer.hpp"

#include "File.hpp"
#include "TextureLoader.hpp"

#include "glad/glad.h"

#include <iostream>
#include <random>

namespace Engine {

MasterRenderer::MasterRenderer(unsigned int width, unsigned int height)
    : m_Viewport{width, height}, m_Framebuffer(Framebuffer::createDefault()), m_State{.framebuffer = m_Framebuffer} {
    m_OutFramebuffer = m_Framebuffer;

    auto vertexSrc = File::read("./shaders/direct-vertex-shader.glsl");
    auto fragmentSrc = File::read("./shaders/direct-fragment-shader.glsl");
    m_DefaultShader = Shader(vertexSrc, fragmentSrc);

    fragmentSrc = File::read("./shaders/direct-fragment-shader-spot-light.glsl");
    m_ShaderWithSpotLight = Shader(vertexSrc, fragmentSrc);

    fragmentSrc = File::read("./shaders/direct-fragment-shader-pbr.glsl");
    m_DefaultShaderPBR = Shader(vertexSrc, fragmentSrc);

    fragmentSrc = File::read("./shaders/direct-fragment-shader-spot-light-pbr.glsl");
    m_ShaderWithSpotLightPBR = Shader(vertexSrc, fragmentSrc);

    vertexSrc = File::read("./shaders/gamma-vertex-shader.glsl");
    fragmentSrc = File::read("./shaders/gamma-fragment-shader.glsl");
    m_GammaShader = Shader(vertexSrc, fragmentSrc);

    vertexSrc = File::read("./shaders/hdr-vertex-shader.glsl");
    fragmentSrc = File::read("./shaders/hdr-fragment-shader.glsl");
    m_HdrShader = Shader(vertexSrc, fragmentSrc);

    m_TmpFramebuffer = Framebuffer::create();
    m_TmpFramebuffer.bind();
    m_TmpColorBuffer = Texture::createRGBA16FBuffer(width, height);
    m_TmpEntityBuffer = Texture::createR32IBuffer(width, height);
    m_TmpFramebuffer.addAttachment(m_TmpColorBuffer);
    m_TmpFramebuffer.addAttachment(m_TmpEntityBuffer);
    m_TmpFramebuffer.setDepthAttachment(
        Renderbuffer::create(width, height, Renderbuffer::InternalFormat::DEPTH_COMPONENT), true);

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

    vertexSrc = File::read("./shaders/blur-simple-vertex-shader.glsl");
    fragmentSrc = File::read("./shaders/blur-simple-fragment-shader.glsl");
    m_BlurSimpleShader = Shader(vertexSrc, fragmentSrc);

    vertexSrc = File::read("./shaders/blur-gaussian-vertex-shader.glsl");
    fragmentSrc = File::read("./shaders/blur-gaussian-fragment-shader.glsl");
    m_BlurGaussianShader = Shader(vertexSrc, fragmentSrc);

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
    m_ParticlesRenderer = std::make_unique<ParticlesRenderer>();
    m_GRenderer = std::make_unique<GRenderer>(*m_ModelRenderer, *m_SkyboxRenderer, *m_ParticlesRenderer);
    m_DirectedLightRenderer = std::make_unique<DirectedLightRenderer>(m_Viewport, *m_ModelRenderer);
    m_DeferredRenderer = std::make_unique<DeferredRenderer>(*m_DirectedLightRenderer, *m_QuadRenderer);
    m_SpotLightRenderer = std::make_unique<SpotLightRenderer>(m_Viewport, *m_ModelRenderer);
    m_WaterRenderer = std::make_unique<WaterRenderer>(m_Viewport, *m_GRenderer, *m_DeferredRenderer);
    m_FlareRenderer = std::make_unique<FlareRenderer>(m_Viewport, *m_QuadRenderer);
    m_Renderer2D = std::make_unique<Renderer2D>(m_Viewport);
    m_FontRenderer = std::make_unique<FontRenderer>(m_Viewport);

    vertexSrc = File::read("./shaders/ssao-vertex-shader.glsl");
    fragmentSrc = File::read("./shaders/ssao-fragment-shader.glsl");
    m_SSAOShader = Shader(vertexSrc, fragmentSrc);

    m_SSAOShader.bind();
    std::uniform_real_distribution<float> randomFloats(0.0, 1.0);
    std::default_random_engine generator;
    for (unsigned int i = 0; i < 64; ++i) {
        glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0,
                         randomFloats(generator));
        sample = glm::normalize(sample);
        sample *= randomFloats(generator);
        float scale = (float)i / 64.0;
        scale = glm::mix(0.1f, 1.0f, scale * scale);
        sample *= scale;
        m_SSAOShader.setFloat3("u_samples[" + std::to_string(i) + "]", sample);
    }

    std::vector<glm::vec3> ssaoNoise;
    for (unsigned int i = 0; i < 16; i++) {
        glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f);
        ssaoNoise.push_back(noise);
    }

    m_SSAONoiseTexture = Texture::createRGB16FBuffer(4, 4, ssaoNoise.data());
    m_SSAONoiseTexture.bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    m_SSAONoiseTexture.unbind();

    m_SSAOShader.setTexture("u_noiseMap", m_SSAONoiseTexture);
    m_SSAOShader.unbind();

    m_SSAOBuffer = Texture::createR16FBuffer(width, height);
    m_SSAOFramebuffer = Framebuffer::create();
    m_SSAOFramebuffer.bind();
    m_SSAOFramebuffer.addAttachment(m_SSAOBuffer);
    m_SSAOFramebuffer.setDepthAttachment(
        Renderbuffer::create(width, height, Renderbuffer::InternalFormat::DEPTH_COMPONENT), true);
    m_SSAOFramebuffer.unbind();

    m_GColorAttachment = Texture::createRGBA16FBuffer(width, height);
    m_GPositionAttachment = Texture::createRGB16FBuffer(width, height);
    m_GNormalAttachment = Texture::createRGB16FBuffer(width, height);
    m_GSpecularAttachment = Texture::createRGBA16FBuffer(width, height);

    m_GFramebuffer = Framebuffer::create();
    m_GFramebuffer.bind();
    m_GFramebuffer.addAttachment(m_GColorAttachment);
    m_GFramebuffer.addAttachment(m_GPositionAttachment);
    m_GFramebuffer.addAttachment(m_GNormalAttachment);
    m_GFramebuffer.addAttachment(m_GSpecularAttachment);
    m_GFramebuffer.setDepthAttachment(
        Renderbuffer::create(width, height, Renderbuffer::InternalFormat::DEPTH_COMPONENT), true);
    m_GFramebuffer.unbind();

    m_BlurAttachment = Texture::createR16FBuffer(width, height);
    m_BlurAttachment.bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    m_BlurAttachment.unbind();

    m_BlurFramebuffer = Framebuffer::create();
    m_BlurFramebuffer.bind();
    m_BlurFramebuffer.addAttachment(m_BlurAttachment);
    m_BlurFramebuffer.setDepthAttachment(
        Renderbuffer::create(width, height, Renderbuffer::InternalFormat::DEPTH_COMPONENT), true);
    m_BlurFramebuffer.unbind();
}

MasterRenderer::~MasterRenderer() {
    m_Framebuffer.free();
    m_TmpFramebuffer.free();
    m_HdrFramebuffer.free();

    m_TmpColorBuffer.free();
    m_TmpEntityBuffer.free();

    m_PingpongFramebuffer[0].free();
    m_PingpongFramebuffer[1].free();

    m_PingpongColorBuffer[0].free();
    m_PingpongColorBuffer[1].free();

    m_DefaultShader.free();
    m_ShaderWithSpotLight.free();
    m_HdrShader.free();
    m_BlurGaussianShader.free();

    m_ColorBuffer[0].free();
    m_ColorBuffer[1].free();
    m_EntityBuffer.free();
    m_DepthRenderbuffer.free();

    m_GColorAttachment.free();
    m_GPositionAttachment.free();
    m_GNormalAttachment.free();
    m_GSpecularAttachment.free();
}

Shader &MasterRenderer::resolveShader(Scene &scene, RenderSettings settings) {
    if (settings.pbr) {
        return scene.getSpotLights().empty() ? m_DefaultShaderPBR : m_ShaderWithSpotLightPBR;
    }
    return scene.getSpotLights().empty() ? m_DefaultShader : m_ShaderWithSpotLight;
}

void MasterRenderer::draw(Camera &camera, Scene &scene, const ModelManager &models, RenderSettings settings) {
    if (settings.gamma) {
        m_State.framebuffer = m_TmpFramebuffer;
    }

    m_Viewport.use();

    if (settings.ssao) {
        m_GFramebuffer.bind();
        m_GRenderer->draw(camera, scene, models, settings);
        m_GFramebuffer.unbind();

        m_SSAOFramebuffer.bind();
        m_SSAOShader.bind();

        m_SSAOShader.setMatrix4("u_view", camera.viewMatrix());
        m_SSAOShader.setMatrix4("u_projection", camera.projectionMatrix());
        m_SSAOShader.setFloat2("u_noiseScale", static_cast<float>(m_Viewport.width) / settings.ssaoNoiseScale,
                               static_cast<float>(m_Viewport.height) / settings.ssaoNoiseScale);
        m_SSAOShader.setInt("u_kernelSize", settings.ssaoKernelSize);
        m_SSAOShader.setFloat("u_radius", settings.ssaoRadius);
        m_SSAOShader.setFloat("u_bias", settings.ssaoBias);

        m_SSAOShader.setTexture("u_positionMap", m_GPositionAttachment);
        m_SSAOShader.setTexture("u_normalMap", m_GNormalAttachment);

        m_QuadRenderer->draw();
        m_SSAOFramebuffer.unbind();

        m_BlurSimpleShader.bind();

        m_BlurSimpleShader.setTexture("u_input", m_SSAOBuffer);
        m_BlurSimpleShader.setInt("u_size", 4);

        m_BlurFramebuffer.bind();
        m_QuadRenderer->draw();
        m_BlurFramebuffer.unbind();
    }

    if (settings.hdr) {
        m_State.framebuffer = m_HdrFramebuffer;
    }
    m_State.framebuffer.bind();

    auto &shader = resolveShader(scene, settings);

    shader.bind();
    shader.setFloat3("u_viewPos", camera.positionVec());
    shader.setMatrix4("u_view", camera.viewMatrix());
    shader.setMatrix4("u_projection", camera.projectionMatrix());
    shader.setFloat("u_threshold", settings.threshold);

    shader.setInt("u_hasNormalMapping", settings.normalMapping);

    shader.setInt("u_fog", settings.fog);
    shader.setFloat3("u_fogColor", settings.fogColor);
    shader.setFloat("u_density", settings.fogDensity);
    shader.setFloat("u_gradient", settings.fogGradient);

    shader.setInt("u_hasDirectedLight", 0);
    shader.setInt("u_spotLightsNumber", 0);

    if (scene.hasDirectedLight()) {
        shader.setInt("u_hasDirectedLight", 1);
        m_DirectedLightRenderer->apply(camera, shader, scene, models, m_State);
    }

    for (const auto &obj : scene.getSpotLights()) {
        m_SpotLightRenderer->apply(obj.light, obj.position, shader, scene, models, m_State);
    }

    m_SkyboxRenderer->draw(camera, scene, settings);

    for (const auto &obj : scene.getParticleEmitters()) {
        m_ParticlesRenderer->draw(obj.particles, obj.position, camera, settings);
    }

    shader.bind();
    if (settings.ssao) {
        shader.setInt("u_hasSSAO", 1);
        shader.setTexture("u_ssao", m_BlurAttachment);
    } else {
        shader.setInt("u_hasSSAO", 0);
    }

    m_ModelRenderer->draw(shader, scene, models);
    // m_WaterRenderer->draw(camera, scene, models, m_State, settings);

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
            m_BlurGaussianShader.bind();
            for (unsigned int i = 0; i < settings.blur; i++) {
                m_PingpongFramebuffer[horizontal].bind();
                m_BlurGaussianShader.setInt("u_horizontal", horizontal);
                if (firstIteration) {
                    m_BlurGaussianShader.setTexture("u_colorBuffer", m_ColorBuffer[1]);
                    firstIteration = false;
                } else {
                    m_BlurGaussianShader.setTexture("u_colorBuffer", m_PingpongColorBuffer[!horizontal]);
                }
                m_QuadRenderer->draw();
                horizontal = !horizontal;
            }
            m_Viewport.resize(lastViewportWidth, lastViewportHeight);
        }

        m_State.framebuffer = settings.gamma ? m_TmpFramebuffer : m_Framebuffer;
        m_State.framebuffer.bind();

        m_HdrShader.bind();
        m_HdrShader.setTexture("u_hdrBuffer", m_ColorBuffer[0]);
        m_HdrShader.setTexture("u_blurBuffer", m_PingpongColorBuffer[horizontal]);
        m_HdrShader.setTexture("u_id", m_EntityBuffer);
        m_HdrShader.setFloat("u_exposure", settings.exposure);
        m_HdrShader.setInt("u_toneMapping", static_cast<int>(settings.toneMapping));

        m_QuadRenderer->draw();
    }

    m_OverlayRenderer->draw(camera, scene, models);

    if (settings.gamma) {
        m_Framebuffer.bind();
        m_GammaShader.bind();
        m_GammaShader.setTexture("u_colorBuffer", m_TmpColorBuffer);
        m_GammaShader.setTexture("u_id", m_TmpEntityBuffer);
        m_GammaShader.setFloat("u_gamma", settings.gammaValue);
        m_QuadRenderer->draw();
    }

    m_State.framebuffer = m_Framebuffer;
    m_State.framebuffer.bind();
    for (const auto &text : scene.getTexts()) {
        m_FontRenderer->draw(text.id, text.text, *text.font, text.transform, text.ndcTransform, text.color);
    }
    m_State.framebuffer.unbind();
}

void MasterRenderer::setClearColor(glm::vec4 color) {
    glClearColor(color.r, color.g, color.b, color.a);
    m_ClearColor = color;
}

glm::vec4 MasterRenderer::getClearColor() { return m_ClearColor; }

void MasterRenderer::setViewport(int width, int height) {
    m_Viewport.width = width;
    m_Viewport.height = height;
    m_DirectedLightRenderer->resize();

    m_TmpColorBuffer.bind();
    m_TmpColorBuffer.resize(width, height);

    m_TmpEntityBuffer.bind();
    m_TmpEntityBuffer.resize(width, height);

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

    m_GColorAttachment.bind();
    m_GColorAttachment.resize(width, height);

    m_GPositionAttachment.bind();
    m_GPositionAttachment.resize(width, height);

    m_GNormalAttachment.bind();
    m_GNormalAttachment.resize(width, height);

    m_GSpecularAttachment.bind();
    m_GSpecularAttachment.resize(width, height);

    m_SSAOBuffer.bind();
    m_SSAOBuffer.resize(width, height);

    m_BlurAttachment.bind();
    m_BlurAttachment.resize(width, height);
    m_BlurAttachment.unbind();

    m_GFramebuffer.resize(width, height);
    m_BlurFramebuffer.resize(width, height);
    m_SSAOFramebuffer.resize(width, height);

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

    m_TmpFramebuffer.bind();
    m_TmpFramebuffer.clear();

    m_HdrFramebuffer.bind();
    m_HdrFramebuffer.clear();
    m_HdrFramebuffer[2].clear({0.0f, 0.0f, 0.0f, 1.0f});

    m_GFramebuffer.bind();
    m_GFramebuffer.clear();

    m_SSAOFramebuffer.bind();
    m_SSAOFramebuffer.clear();

    m_BlurFramebuffer.bind();
    m_BlurFramebuffer.clear();

    m_Framebuffer.bind();
    m_Framebuffer.clear();
    m_Framebuffer.unbind();
}

void MasterRenderer::draw2D(Texture &texture, glm::vec2 position, glm::vec2 size, glm::vec4 color, uint32_t id) {
    m_Framebuffer.bind();
    m_Renderer2D->draw(texture, position, size, color, id);
    m_Framebuffer.unbind();
}

void MasterRenderer::draw2D(const std::vector<Mesh2D::Vertex> &vertices, std::vector<uint32_t> &indices,
                            Texture &texture, const glm::mat4 &model, uint32_t id) {
    m_Framebuffer.bind();
    m_Renderer2D->draw(vertices, indices, texture, model, id);
    m_Framebuffer.unbind();
}

} // namespace Engine