#include "SpotLightRenderer.hpp"

#include "File.hpp"

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
                              const ModelManager &models, RendererState &state) {
    glBindFramebuffer(GL_FRAMEBUFFER, m_DepthCubeMapFBO);
    glViewport(0, 0, 1024, 1024);

    auto &cubeMap = m_SadowCubeMaps[m_ActiveLights];

    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, cubeMap->texture().getId(), 0);
    glClear(GL_DEPTH_BUFFER_BIT);

    m_CubeShadowShader->bind();
    cubeMap->setPosition(position);
    cubeMap->bind(*m_CubeShadowShader);

    RendererState rs;
    rs.activeTextures = 1;
    rs.fbo = m_DepthCubeMapFBO;
    m_ModelRenderer.draw(*m_CubeShadowShader, scene, models, rs);

    glBindFramebuffer(GL_FRAMEBUFFER, state.fbo);
    glViewport(0, 0, m_Viewport.width, m_Viewport.height);
    // glCullFace(GL_BACK);

    std::string lightRef = "u_spotLights[" + std::to_string(m_ActiveLights) + "]";

    shader.bind();
    shader.setFloat3(lightRef + ".position", position);
    shader.setFloat3(lightRef + ".ambient", light.ambient);
    shader.setFloat3(lightRef + ".diffuse", light.diffuse);
    shader.setFloat3(lightRef + ".specular", light.specular);
    shader.setFloat(lightRef + ".constant", light.constant);
    shader.setFloat(lightRef + ".linear", light.linear);
    shader.setFloat(lightRef + ".quadratic", light.quadratic);

    glActiveTexture(GL_TEXTURE0 + state.activeTextures);
    m_SadowCubeMaps[m_ActiveLights]->texture().bind();
    shader.setInt(lightRef + ".shadowMap", static_cast<int>(state.activeTextures));
    ++state.activeTextures;

    shader.setFloat(lightRef + ".farPlane", m_SadowCubeMaps[m_ActiveLights]->farPlane());

    m_ActiveLights++;
    shader.setInt("u_spotLightsNumber", static_cast<int>(m_ActiveLights));
}

void SpotLightRenderer::clear() { m_ActiveLights = 0; }

} // namespace Engine