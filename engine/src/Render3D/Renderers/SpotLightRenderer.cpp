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
    m_CubeShadowShader = Shader(vertexSrc, fragmentSrc, geometrySrc);

    for (size_t i = 0; i < 4; i++) {
        m_SadowCubeMaps[i] = std::make_unique<CubeMap>(1024, 1024, 50.0f, glm::vec3(0.0f, 0.0f, 0.0f));
    }

    m_DepthCubeMapFramebuffer = Framebuffer::create();
}

void SpotLightRenderer::apply(const SpotLight &light, const glm::vec3 &position, Shader &shader, Scene &scene,
                              const ModelManager &models, RendererState &state) {
    unsigned int lastViewportWidth = m_Viewport.width;
    unsigned int lastViewportHeight = m_Viewport.height;

    m_DepthCubeMapFramebuffer.bind();
    m_Viewport.resize(1024, 1024);

    auto &cubeMap = m_SadowCubeMaps[m_ActiveLights];
    m_DepthCubeMapFramebuffer.setDepthAttachment(cubeMap->texture());
    m_DepthCubeMapFramebuffer.clearDepth();

    m_CubeShadowShader.bind();
    cubeMap->setPosition(position);
    cubeMap->bind(m_CubeShadowShader);

    m_DepthCubeMapFramebuffer.bind();
    m_ModelRenderer.draw(m_CubeShadowShader, scene, models);

    state.framebuffer.bind();
    m_Viewport.resize(lastViewportWidth, lastViewportHeight);
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
    shader.setTexture(lightRef + ".shadowMap", m_SadowCubeMaps[m_ActiveLights]->texture());
    shader.setFloat(lightRef + ".farPlane", m_SadowCubeMaps[m_ActiveLights]->farPlane());

    m_ActiveLights++;
    shader.setInt("u_spotLightsNumber", static_cast<int>(m_ActiveLights));
}

void SpotLightRenderer::clear() { m_ActiveLights = 0; }

} // namespace Engine