#include "WaterRenderer.hpp"

#include "File.hpp"
#include "TextureLoader.hpp"

#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp>

namespace Engine {

WaterRenderer::WaterRenderer(Viewport &viewport, GRenderer &gRenderer, DeferredRenderer &deferredRenderer)
    : m_Viewport(viewport), m_GRenderer(gRenderer), m_DeferredRenderer(deferredRenderer) {
    auto vertexSrc = File::read("./shaders/water-vertex-shader.glsl");
    auto fragmentSrc = File::read("./shaders/water-fragment-shader.glsl");
    m_Shader = Shader(vertexSrc, fragmentSrc);

    m_WaterDudvMap = TextureLoader::loadTexture("./shaders/waterDUDV.png");
    m_WaterNormalMap = TextureLoader::loadTexture("./shaders/waterNormalMap.png");

    // clang-format off
    float waterVertices[] = {
        -1.0f,  0.0f, -1.0f, 0.0f, 1.0f,
        -1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f,  0.0f, -1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    };

    // clang-format on
    glGenVertexArrays(1, &m_WaterVAO);
    glGenBuffers(1, &m_WaterVBO);
    glBindVertexArray(m_WaterVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_WaterVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(waterVertices), &waterVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    m_ReflectionColorAttachment = Texture::createRGBA16FBuffer(c_ReflectionWidth, c_ReflectionHeight);
    m_RefractionColorAttachment = Texture::createRGBA16FBuffer(c_RefractionWidth, c_RefractionHeight);
    m_PositionAttachment = Texture::createRGB16FBuffer(c_RefractionWidth, c_RefractionHeight);
    m_NormalAttachment = Texture::createRGB16FBuffer(c_RefractionWidth, c_RefractionHeight);
    m_SpecularAttachment = Texture::createRGBA16FBuffer(c_RefractionWidth, c_RefractionHeight);
    m_DepthAttachment = Texture::createDepthBuffer(c_RefractionWidth, c_RefractionHeight);

    m_ReflectionFbo = Framebuffer::create();
    m_ReflectionFbo.bind();
    m_ReflectionFbo.addAttachment(m_ReflectionColorAttachment);
    m_ReflectionFbo.setDepthAttachment(
        Renderbuffer::create(c_ReflectionWidth, c_ReflectionHeight, Renderbuffer::InternalFormat::DEPTH_COMPONENT));

    m_RefractionFbo = Framebuffer::create();
    m_RefractionFbo.bind();
    m_RefractionFbo.addAttachment(m_RefractionColorAttachment);
    m_RefractionFbo.addAttachment(m_PositionAttachment);
    m_RefractionFbo.addAttachment(m_NormalAttachment);
    m_RefractionFbo.addAttachment(m_SpecularAttachment);
    m_RefractionFbo.setDepthAttachment(m_DepthAttachment);
    m_RefractionFbo.unbind();
}

void WaterRenderer::draw(Camera &camera, Scene &scene, const ModelManager &models, RendererState &state,
                         RenderSettings &settings) {
    unsigned int lastViewportWidth = m_Viewport.width;
    unsigned int lastViewportHeight = m_Viewport.height;

    glm::vec4 lastClipPlane = settings.clipPlane;

    // Reflection begin
    camera.setPosition(camera.positionVec() * glm::vec3(1.0, -1.0, 1.0));
    camera.inversePitch();

    settings.clipPlane = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);

    m_Viewport.resize(c_ReflectionWidth, c_ReflectionHeight);

    m_ReflectionFbo.bind();
    m_ReflectionFbo.clear();
    m_GRenderer.draw(camera, scene, models, settings);

    camera.inversePitch();
    camera.setPosition(camera.positionVec() * glm::vec3(1.0, -1.0, 1.0));
    // Reflection end

    // Refraction begin
    camera.setFieldOfView(glm::radians(45.0f * 1.5f));

    settings.clipPlane = glm::vec4(0.0f, -1.0f, 0.0f, 0.0f);

    m_Viewport.resize(c_RefractionWidth, c_RefractionHeight);
    m_RefractionFbo.bind();
    m_RefractionFbo.clear();
    m_GRenderer.draw(camera, scene, models, settings);

    camera.setFieldOfView(glm::radians(45.0f));
    // Refraction end

    settings.clipPlane = lastClipPlane;

    m_Viewport.resize(lastViewportWidth, lastViewportHeight);

    m_Shader.bind();

    glm::mat4 model(1);
    model = glm::translate(model, glm::vec3(glm::vec3(0.0, 0.0, 0.0)));
    model = glm::scale(model, glm::vec3(10.0));

    m_Shader.setFloat3("u_viewPos", camera.positionVec());
    m_Shader.setMatrix4("u_view", camera.viewMatrix());
    m_Shader.setMatrix4("u_projection", camera.projectionMatrix());
    m_Shader.setMatrix4("u_model", model);
    m_Shader.setMatrix4("u_noramlFix", glm::rotate(-1.57f, glm::vec3(1.0f, 0.0f, 0.0f)));

    m_Shader.setFloat("u_threshold", settings.threshold);

    m_Shader.setFloat("u_moveFactor", m_WaterMoveFactor);
    m_WaterMoveFactor += 0.001f;

    auto &light = scene.getDirectedLight();
    glm::vec3 lightDir = glm::quat(light.rotation) * glm::vec3(0.0f, -1.0f, 0.0f);
    m_Shader.setFloat3("u_directedLight.direction", lightDir);
    m_Shader.setFloat3("u_directedLight.ambient", light.ambient * light.intensity);
    m_Shader.setFloat3("u_directedLight.diffuse", light.diffuse * light.intensity);
    m_Shader.setFloat3("u_directedLight.specular", light.specular * light.intensity);

    m_Shader.setTexture("u_reflectionMap", m_ReflectionColorAttachment);
    m_Shader.setTexture("u_refractionMap", m_RefractionColorAttachment);
    m_Shader.setTexture("u_depthMap", m_DepthAttachment);
    m_Shader.setTexture("u_positionMap", m_PositionAttachment);
    m_Shader.setTexture("u_dudvMap", m_WaterDudvMap);
    m_Shader.setTexture("u_normalMap", m_WaterNormalMap);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    state.framebuffer.bind();

    glBindVertexArray(m_WaterVBO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 5);
    glBindVertexArray(0);

    glDisable(GL_BLEND);
}

} // namespace Engine