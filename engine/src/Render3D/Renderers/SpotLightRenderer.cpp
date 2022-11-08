#include "SpotLightRenderer.hpp"

#include "File.hpp"

#include "glad/glad.h"
#include <glm/glm.hpp>

namespace Engine {

SpotLightRenderer::SpotLightRenderer(Viewport &viewport, ModelRenderer &modelRenderer)
    : m_Viewport(viewport), m_ModelRenderer(modelRenderer) {
    auto vertexSrc = File::readGLSL("./shaders/pass/cube-shadow.vertex.glsl");
    auto fragmentSrc = File::readGLSL("./shaders/pass/cube-shadow.fragment.glsl");
    auto geometrySrc = File::readGLSL("./shaders/pass/cube-shadow.geometry.glsl");
    m_CubeShadowShader = Shader(vertexSrc, fragmentSrc, geometrySrc);

    for (size_t i = 0; i < 4; i++) {
        m_SadowCubeMaps[i] = CubeMap(1024, 1024, 0.1f, 50.0f, glm::vec3(0.0f, 0.0f, 0.0f));
    }

    m_DepthCubeMapFramebuffer = Framebuffer::create();
}

SpotLightRenderer::~SpotLightRenderer() {
    m_CubeShadowShader.free();
    m_DepthCubeMapFramebuffer.free();
    for (size_t i = 0; i < 4; i++) {
        m_SadowCubeMaps[i].free();
    }
}

void SpotLightRenderer::prepareContext(RenderContext& context) {
    for (size_t i = 0; i < 4; i++) {
        std::string ref = "u_spotLights[" + std::to_string(i) + "].shadowMap";
        context.baseMaterial.setTexture(ref, &m_SadowCubeMaps[i].texture());
    }
}

void SpotLightRenderer::apply(const SpotLight &light, const glm::vec3 &position, Scene &scene,
                              const ModelManager &models, RendererState &state) {
    unsigned int lastViewportWidth = m_Viewport.width;
    unsigned int lastViewportHeight = m_Viewport.height;

    m_DepthCubeMapFramebuffer.bind();
    m_Viewport.resize(light.shadowFrustumWidth, light.shadowFrustumHeight);

    auto &cubeMap = m_SadowCubeMaps[m_ActiveLights];
    m_DepthCubeMapFramebuffer.setDepthAttachment(cubeMap.texture());
    m_DepthCubeMapFramebuffer.clearDepth();

    m_CubeShadowShader.bind();
    cubeMap.set(light.shadowFrustumWidth, light.shadowFrustumHeight, light.shadowFrustumNearPlane,
                light.shadowFrustumFarPlane);
    cubeMap.setPosition(position);
    cubeMap.bind(m_CubeShadowShader);

    m_ModelRenderer.draw(scene, models, nullptr, &m_CubeShadowShader);

    state.framebuffer.bind();
    m_Viewport.resize(lastViewportWidth, lastViewportHeight);
    // glCullFace(GL_BACK);

    std::string lightRef = "u_spotLights[" + std::to_string(m_ActiveLights) + "]";

    state.baseMaterial->setFloat3(lightRef + ".position", position);
    state.baseMaterial->setFloat3(lightRef + ".ambient", light.ambient * light.intensity);
    state.baseMaterial->setFloat3(lightRef + ".diffuse", light.diffuse * light.intensity);
    state.baseMaterial->setFloat3(lightRef + ".specular", light.specular * light.intensity);
    state.baseMaterial->setFloat(lightRef + ".constant", light.constant);
    state.baseMaterial->setFloat(lightRef + ".linear", light.linear);
    state.baseMaterial->setFloat(lightRef + ".quadratic", light.quadratic);
    state.baseMaterial->setTexture(lightRef + ".shadowMap", &m_SadowCubeMaps[m_ActiveLights].texture());
    state.baseMaterial->setFloat(lightRef + ".farPlane", m_SadowCubeMaps[m_ActiveLights].farPlane());
    state.baseMaterial->setFloat(lightRef + ".bias", light.bias);
    state.baseMaterial->setInt(lightRef + ".pcfSamples", light.pcfSamples);
    state.baseMaterial->setFloat(lightRef + ".pcfDiskRadius", light.pcfDiskRadius);

    m_ActiveLights++;
    state.baseMaterial->setInt("u_spotLightsNumber", static_cast<int>(m_ActiveLights));
}

void SpotLightRenderer::clear() { m_ActiveLights = 0; }

} // namespace Engine