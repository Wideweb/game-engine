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
    : m_Viewport(viewport), m_ModelRenderer(modelRenderer) {
    auto vertexSrc = File::read("./shaders/depth-vertex-shader.glsl");
    auto fragmentSrc = File::read("./shaders/depth-fragment-shader.glsl");
    m_DepthShader = std::make_unique<Shader>(vertexSrc, fragmentSrc);

    m_DepthMap = Texture::createDepthBuffer(viewport.width * 2.0f, viewport.height * 2.0f);

    m_DepthMapFramebuffer = Framebuffer::create();
    m_DepthMapFramebuffer.bind();
    m_DepthMapFramebuffer.setDepthAttachment(m_DepthMap);
    m_DepthMapFramebuffer.unbind();
}

void DirectedLightRenderer::apply(Camera &camera, const DirectedLight &light, Shader &shader, Scene &scene,
                                  const ModelManager &models, RendererState &state) {
    unsigned int lastViewportWidth = m_Viewport.width;
    unsigned int lastViewportHeight = m_Viewport.height;

    m_DepthMapFramebuffer.bind();
    m_Viewport.resize(m_Viewport.width * 2.0f, m_Viewport.height * 2.0f);
    m_DepthMapFramebuffer.clearDepth();

    float farHalf = light.farPlane / 2.0f;
    glm::vec3 direction = glm::quat(light.rotation) * glm::vec3(0.0f, -1.0f, 0.0f);

    glm::vec3 position = camera.positionVec() - direction * farHalf + camera.frontVec() * farHalf;

    glm::mat4 lightProjection = glm::ortho(-farHalf, farHalf, -farHalf, farHalf, light.nearPlane, light.farPlane);
    glm::mat4 lightView = glm::lookAt(position, position + direction, glm::vec3(0.0f, 0.0f, 1.0f));

    m_DepthShader->bind();
    m_DepthShader->setMatrix4("u_view", lightView);
    m_DepthShader->setMatrix4("u_projection", lightProjection);

    RendererState rs = {.framebuffer = m_DepthMapFramebuffer};

    // glCullFace(GL_FRONT);
    m_ModelRenderer.draw(*m_DepthShader, scene, models);
    // glCullFace(GL_BACK);

    state.framebuffer.bind();
    m_Viewport.resize(lastViewportWidth, lastViewportHeight);

    shader.bind();
    shader.setInt("u_directedLight.pcf", light.pcf);
    shader.setFloat("u_directedLight.biasFactor", light.biasFactor);
    shader.setFloat("u_directedLight.biasMin", light.biasMin);
    shader.setFloat3("u_directedLight.direction", direction);
    shader.setFloat3("u_directedLight.ambient", light.ambient * light.intensity);
    shader.setFloat3("u_directedLight.diffuse", light.diffuse * light.intensity);
    shader.setFloat3("u_directedLight.specular", light.specular * light.intensity);
    shader.setMatrix4("u_directedLight.spaceMatrix", lightProjection * lightView);
    shader.setTexture("u_directedLight.shadowMap", m_DepthMap);
}

void DirectedLightRenderer::resize() {
    m_DepthMap.bind();
    m_DepthMap.resize(m_Viewport.width * 2.0f, m_Viewport.height * 2.0f);
    m_DepthMap.unbind();
}

} // namespace Engine