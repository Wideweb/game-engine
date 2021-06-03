#include "DirectedLightRenderer.hpp"

#include "File.hpp"
#include "TextureLoader.hpp"

#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp>

namespace Engine {

DirectedLightRenderer::DirectedLightRenderer(Viewport &viewport, ModelRenderer &modelRenderer)
    : m_ViewPort(viewport), m_ModelRenderer(modelRenderer) {
    auto vertexSrc = File::read("./shaders/depth-vertex-shader.glsl");
    auto fragmentSrc = File::read("./shaders/depth-fragment-shader.glsl");
    m_DepthShader = std::make_unique<Shader>(vertexSrc, fragmentSrc);

    m_DepthMap.reset(TextureLoader::createDepthBuffer(1024, 1024));

    glGenFramebuffers(1, &m_DepthMapFBO);

    glBindFramebuffer(GL_FRAMEBUFFER, m_DepthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthMap->getId(), 0);

    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DirectedLightRenderer::apply(Camera &camera, const DirectedLight &light, Shader &shader, Scene &scene,
                                  const ModelManager &models) {
    glBindFramebuffer(GL_FRAMEBUFFER, m_DepthMapFBO);
    glViewport(0, 0, 1024, 1024);
    glClear(GL_DEPTH_BUFFER_BIT);

    float farHalf = light.farPlane / 2.0f;

    glm::vec3 position = camera.positionVec() - light.direction * farHalf;

    glm::mat4 lightProjection = glm::ortho(-farHalf, farHalf, -farHalf, farHalf, light.nearPlane, light.farPlane);
    glm::mat4 lightView = glm::lookAt(position, position + light.direction, glm::vec3(0.0f, 0.0f, 1.0f));

    m_DepthShader->bind();
    m_DepthShader->setMatrix4("u_view", lightView);
    m_DepthShader->setMatrix4("u_projection", lightProjection);

    m_ModelRenderer.draw(*m_DepthShader, scene, models, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, m_ViewPort.width, m_ViewPort.height);

    shader.bind();
    shader.setFloat3("u_directedLight.direction", light.direction);
    shader.setFloat3("u_directedLight.ambient", light.ambient);
    shader.setFloat3("u_directedLight.diffuse", light.diffuse);
    shader.setFloat3("u_directedLight.specular", light.specular);
    shader.setMatrix4("u_directedLight.spaceMatrix", lightProjection * lightView);

    glActiveTexture(GL_TEXTURE0);
    m_DepthMap->bind();
    shader.setInt("u_directedLight.shadowMap", 0);
}

} // namespace Engine