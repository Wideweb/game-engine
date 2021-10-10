#include "DirectedLightRenderer.hpp"

#include "File.hpp"
#include "TextureLoader.hpp"

#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp>

namespace Engine {

DirectedLightRenderer::DirectedLightRenderer(Viewport &viewport, ModelRenderer &modelRenderer)
    : m_ViewPort(viewport), m_ModelRenderer(modelRenderer) {
    auto vertexSrc = File::read("./shaders/depth-vertex-shader.glsl");
    auto fragmentSrc = File::read("./shaders/depth-fragment-shader.glsl");
    m_DepthShader = std::make_unique<Shader>(vertexSrc, fragmentSrc);

    m_DepthMap.reset(TextureLoader::createDepthBuffer(viewport.width * 2.0f, viewport.height * 2.0f));

    glGenFramebuffers(1, &m_DepthMapFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_DepthMapFBO);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthMap->getId(), 0);

    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DirectedLightRenderer::apply(Camera &camera, const DirectedLight &light, Shader &shader, Scene &scene,
                                  const ModelManager &models, RendererState &state) {
    glBindFramebuffer(GL_FRAMEBUFFER, m_DepthMapFBO);
    glViewport(0, 0, m_ViewPort.width * 2.0f, m_ViewPort.height * 2.0f);
    glClear(GL_DEPTH_BUFFER_BIT);

    float farHalf = light.farPlane / 2.0f;
    glm::vec3 direction = glm::quat(light.rotation) * glm::vec3(0.0f, -1.0f, 0.0f);

    glm::vec3 position = camera.positionVec() - direction * farHalf + camera.frontVec() * farHalf;

    glm::mat4 lightProjection = glm::ortho(-farHalf, farHalf, -farHalf, farHalf, light.nearPlane, light.farPlane);
    glm::mat4 lightView = glm::lookAt(position, position + direction, glm::vec3(0.0f, 0.0f, 1.0f));

    m_DepthShader->bind();
    m_DepthShader->setMatrix4("u_view", lightView);
    m_DepthShader->setMatrix4("u_projection", lightProjection);

    RendererState rs;
    rs.activeTextures = 0;
    rs.fbo = m_DepthMapFBO;

    // glCullFace(GL_FRONT);
    m_ModelRenderer.draw(*m_DepthShader, scene, models, rs);
    // glCullFace(GL_BACK);

    glBindFramebuffer(GL_FRAMEBUFFER, state.fbo);
    glViewport(0, 0, m_ViewPort.width, m_ViewPort.height);

    shader.bind();
    shader.setInt("u_directedLight.pcf", light.pcf);
    shader.setFloat("u_directedLight.biasFactor", light.biasFactor);
    shader.setFloat("u_directedLight.biasMin", light.biasMin);
    shader.setFloat3("u_directedLight.direction", direction);
    shader.setFloat3("u_directedLight.ambient", light.ambient * light.intensity);
    shader.setFloat3("u_directedLight.diffuse", light.diffuse * light.intensity);
    shader.setFloat3("u_directedLight.specular", light.specular * light.intensity);
    shader.setMatrix4("u_directedLight.spaceMatrix", lightProjection * lightView);

    glActiveTexture(GL_TEXTURE0 + state.activeTextures);
    m_DepthMap->bind();
    shader.setInt("u_directedLight.shadowMap", static_cast<int>(state.activeTextures));
    ++state.activeTextures;
}

void DirectedLightRenderer::resize() {
    m_DepthMap->bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_ViewPort.width * 2.0f, m_ViewPort.height * 2.0f, 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    m_DepthMap->unbind();
}

} // namespace Engine