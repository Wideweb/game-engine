#include "SpotLightRenderer.hpp"

#include "glad/glad.h"
#include <glm/glm.hpp>

namespace Engine {

SpotLightRenderer::SpotLightRenderer(Viewport &viewport, ModelRenderer &modelRenderer)
    : m_Viewport(viewport), m_ModelRenderer(modelRenderer) {
    auto vertexSrc = File::read("./shaders/cube-shadow-vertex-shader.glsl");
    auto fragmentSrc = File::read("./shaders/cube-shadow-fragment-shader.glsl");
    auto geometrySrc = File::read("./shaders/cube-shadow-geometry-shader.glsl");
    m_CubeShadowShader = std::make_unique<Shader>(vertexSrc, fragmentSrc, geometrySrc);

    for (size_t i = 0; i < 4; i++) {
        m_SadowCubeMaps[i] = std::make_unique<CubeMap>(1024, 1024, 50.0f, glm::vec3(0.0f, 0.0f, 0.0f));
    }

    glGenFramebuffers(1, &m_DepthCubeMapFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_DepthCubeMapFBO);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SpotLightRenderer::apply(const SpotLight &light, const glm::vec3 &position, Shader &shader, Scene &scene,
                              const ModelManager &models) {
    glViewport(0, 0, 1024, 1024);
    glBindFramebuffer(GL_FRAMEBUFFER, m_DepthCubeMapFBO);

    auto &cubeMap = m_SadowCubeMaps[m_ActiveLights];

    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, cubeMap->texture().getId(), 0);
    glClear(GL_DEPTH_BUFFER_BIT);

    m_CubeShadowShader->bind();
    cubeMap->setPosition(position);
    cubeMap->bind(*m_CubeShadowShader);

    m_ModelRenderer.draw(*m_CubeShadowShader, scene, models, 1);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, m_Viewport.width, m_Viewport.height);
    // glCullFace(GL_BACK);

    std::string lightRef = "u_spotLights[" + std::to_string(m_ActiveLights) + "]";
    unsigned int textureIndex = m_ActiveLights + 1;

    shader.bind();
    shader.setFloat3(lightRef + ".position", position);
    shader.setFloat3(lightRef + ".ambient", light.ambient);
    shader.setFloat3(lightRef + ".diffuse", light.diffuse);
    shader.setFloat3(lightRef + ".specular", light.specular);
    shader.setFloat(lightRef + ".constant", light.constant);
    shader.setFloat(lightRef + ".linear", light.linear);
    shader.setFloat(lightRef + ".quadratic", light.quadratic);

    glActiveTexture(GL_TEXTURE0 + textureIndex);
    m_SadowCubeMaps[m_ActiveLights]->texture().bind();
    shader.setInt(lightRef + ".shadowMap", static_cast<int>(textureIndex));
    shader.setFloat(lightRef + ".farPlane", m_SadowCubeMaps[m_ActiveLights]->farPlane());

    m_ActiveLights++;
    shader.setInt("u_spotLightsNumber", static_cast<int>(m_ActiveLights));
}

void SpotLightRenderer::clear() { m_ActiveLights = 0; }

} // namespace Engine