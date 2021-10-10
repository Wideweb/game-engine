#include "GRenderer.hpp"

#include "File.hpp"
#include "TextureLoader.hpp"

#include "glad/glad.h"

#include <iostream>

namespace Engine {

GRenderer::GRenderer(Viewport &viewport, ModelRenderer &modelRenderer, SkyboxRenderer &skyboxRenderer)
    : m_Viewport(viewport), m_ModelRenderer(modelRenderer), m_SkyboxRenderer(skyboxRenderer) {
    auto vertexSrc = File::read("./shaders/g-buffer-vertex-shader.glsl");
    auto fragmentSrc = File::read("./shaders/g-buffer-fragment-shader.glsl");
    m_Shader = std::make_unique<Shader>(vertexSrc, fragmentSrc);

    glGenFramebuffers(1, &m_GBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_GBuffer);

    // буфер для цвета
    glGenTextures(1, &m_GColor);
    glBindTexture(GL_TEXTURE_2D, m_GColor);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_Viewport.width, m_Viewport.height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_GColor, 0);

    // буфер позиций
    glGenTextures(1, &m_GPosition);
    glBindTexture(GL_TEXTURE_2D, m_GPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_Viewport.width, m_Viewport.height, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_GPosition, 0);

    // буфер нормалей
    glGenTextures(1, &m_GNormal);
    glBindTexture(GL_TEXTURE_2D, m_GNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_Viewport.width, m_Viewport.height, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_GNormal, 0);

    // буфер для отражение
    glGenTextures(1, &m_GSpecular);
    glBindTexture(GL_TEXTURE_2D, m_GSpecular);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_Viewport.width, m_Viewport.height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_GSpecular, 0);

    // буфер для глубины
    glGenTextures(1, &m_GDepth);
    glBindTexture(GL_TEXTURE_2D, m_GDepth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_Viewport.width, m_Viewport.height, 0, GL_DEPTH_COMPONENT,
                 GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_GDepth, 0);

    // укажем OpenGL, какие буферы мы будем использовать при рендеринге
    unsigned int attachments[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2,
                                   GL_COLOR_ATTACHMENT3};
    glDrawBuffers(4, attachments);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GRenderer::draw(Camera &camera, Scene &scene, const ModelManager &models, RendererState &state,
                     RenderSettings &settings) {
    glBindFramebuffer(GL_FRAMEBUFFER, m_GBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_Shader->bind();
    m_Shader->setMatrix4("u_view", camera.viewMatrix());
    m_Shader->setMatrix4("u_projection", camera.projectionMatrix());
    m_Shader->setFloat("u_clipY", 0);

    RendererState rs;
    rs.activeTextures = 0;
    rs.fbo = m_GBuffer;
    m_ModelRenderer.draw(*m_Shader, scene, models, rs);
    m_SkyboxRenderer.draw(camera, scene, settings);

    glBindFramebuffer(GL_FRAMEBUFFER, state.fbo);
}

void GRenderer::resize() {
    // буфер для цвета
    glBindTexture(GL_TEXTURE_2D, m_GColor);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_Viewport.width, m_Viewport.height, 0, GL_RGBA, GL_FLOAT, NULL);

    // буфер позиций
    glBindTexture(GL_TEXTURE_2D, m_GPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_Viewport.width, m_Viewport.height, 0, GL_RGB, GL_FLOAT, NULL);

    // буфер нормалей
    glBindTexture(GL_TEXTURE_2D, m_GNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_Viewport.width, m_Viewport.height, 0, GL_RGB, GL_FLOAT, NULL);

    // буфер для отражение
    glBindTexture(GL_TEXTURE_2D, m_GSpecular);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_Viewport.width, m_Viewport.height, 0, GL_RGBA, GL_FLOAT, NULL);

    // буфер для глубины
    glBindTexture(GL_TEXTURE_2D, m_GDepth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_Viewport.width, m_Viewport.height, 0, GL_DEPTH_COMPONENT,
                 GL_FLOAT, NULL);
}

} // namespace Engine