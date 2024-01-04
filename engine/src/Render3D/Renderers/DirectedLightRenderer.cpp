#include "DirectedLightRenderer.hpp"

#include "File.hpp"
#include "GLSLPreprocessor.hpp"
#include "TextureLoader.hpp"

#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp>

namespace Engine {

DirectedLightRenderer::DirectedLightRenderer(Viewport &viewport, ModelRenderer &modelRenderer)
    : m_Viewport(viewport), m_ModelRenderer(modelRenderer) {
    auto vertexSrc = GLSLPreprocessor::preprocess("./shaders/pass/depth.vertex.glsl").sourceCode;
    auto fragmentSrc = GLSLPreprocessor::preprocess("./shaders/pass/depth.fragment.glsl").sourceCode;
    m_DepthShader = Shader(vertexSrc, fragmentSrc);

    m_DepthMap = Texture::createDepthBuffer(viewport.width * 2.0f, viewport.height * 2.0f);

    m_DepthMapFramebuffer = Framebuffer::create();
    m_DepthMapFramebuffer.bind();
    m_DepthMapFramebuffer.setDepthAttachment(m_DepthMap);
    m_DepthMapFramebuffer.unbind();
}

DirectedLightRenderer::~DirectedLightRenderer() {
    m_DepthShader.free();
    m_DepthMapFramebuffer.free();
    m_DepthMap.free();
}

void DirectedLightRenderer::prepareContext(RenderContext &context) {
    for (size_t i = 0; i < 4; i++) {
        std::string ref = "u_directedLights[" + std::to_string(i) + "].shadowMap";
        context.baseMaterial.setTexture(ref, &m_DepthMap);
    }
}

void DirectedLightRenderer::apply(Camera &camera, Scene &scene, const ModelManager &models, RendererState &state) {
    const auto &lightObj = scene.getDirectedLight();
    const auto light = lightObj.light;

    unsigned int lastViewportWidth = m_Viewport.width;
    unsigned int lastViewportHeight = m_Viewport.height;

    m_DepthMapFramebuffer.bind();
    m_Viewport.resize(m_Viewport.width * 2.0f, m_Viewport.height * 2.0f);
    m_DepthMapFramebuffer.clearDepth();

    // float farHalf = light.shadowFrustumFarPlane / 2.0f;
    glm::vec3 direction = lightObj.rotation * glm::vec3(0.0f, -1.0f, 0.0f);

    // glm::vec3 position = camera.positionVec() - direction * farHalf + camera.frontVec() * farHalf;

    glm::mat4 lightProjection =
        glm::ortho(light.shadowFrustumWidth * -0.5f, light.shadowFrustumWidth * 0.5f, light.shadowFrustumHeight * -0.5f,
                   light.shadowFrustumHeight * 0.5f, light.shadowFrustumNearPlane, light.shadowFrustumFarPlane);
    glm::mat4 lightView = glm::lookAt(lightObj.position, lightObj.position + direction, glm::vec3(0.0f, 0.0f, 1.0f));

    m_DepthShader.bind();
    m_DepthShader.setMatrix4("u_view", lightView);
    m_DepthShader.setMatrix4("u_projection", lightProjection);

    RendererState rs = {.framebuffer = m_DepthMapFramebuffer};

    // glCullFace(GL_FRONT);
    m_ModelRenderer.draw(scene, models, nullptr, &m_DepthShader);
    // glCullFace(GL_BACK);

    state.framebuffer.bind();
    m_Viewport.resize(lastViewportWidth, lastViewportHeight);

    state.baseMaterial->setInt("u_directedLights[0].pcf", light.pcf);
    state.baseMaterial->setFloat("u_directedLights[0].biasFactor", light.biasFactor);
    state.baseMaterial->setFloat("u_directedLights[0].biasMin", light.biasMin);
    state.baseMaterial->setFloat3("u_directedLights[0].direction", direction);
    state.baseMaterial->setFloat3("u_directedLights[0].ambient", light.ambient * light.intensity);
    state.baseMaterial->setFloat3("u_directedLights[0].diffuse", light.diffuse * light.intensity);
    state.baseMaterial->setFloat3("u_directedLights[0].specular", light.specular * light.intensity);
    state.baseMaterial->setMatrix4("u_directedLights[0].spaceMatrix", lightProjection * lightView);
    state.baseMaterial->setTexture("u_directedLights[0].shadowMap", &m_DepthMap);

    state.baseMaterial->setInt("u_directedLightsNumber", 1);
}

void DirectedLightRenderer::resize() {
    m_DepthMap.bind();
    m_DepthMap.resize(m_Viewport.width * 2.0f, m_Viewport.height * 2.0f);
    m_DepthMap.unbind();
}

} // namespace Engine