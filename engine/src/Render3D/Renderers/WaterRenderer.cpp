#include "WaterRenderer.hpp"

#include "File.hpp"
#include "TextureLoader.hpp"

#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp>

namespace Engine {

WaterRenderer::WaterRenderer(Viewport &viewport, GRenderer &gRenderer) : m_Viewport(viewport), m_GRenderer(gRenderer) {
    auto vertexSrc = File::read("./shaders/water-vertex-shader.glsl");
    auto fragmentSrc = File::read("./shaders/water-fragment-shader.glsl");
    m_WaterShader = std::make_unique<Shader>(vertexSrc, fragmentSrc);

    m_WaterDudvMap.reset(TextureLoader::loadTexture("./shaders/waterDUDV.png"));
    m_WaterNormalMap.reset(TextureLoader::loadTexture("./shaders/waterNormalMap.png"));

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

    // Reflection color
    glGenTextures(1, &m_ReflectionColor);
    glBindTexture(GL_TEXTURE_2D, m_ReflectionColor);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, c_ReflectionWidth, c_ReflectionHeight, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Refraction color
    glGenTextures(1, &m_RefractionColor);
    glBindTexture(GL_TEXTURE_2D, m_RefractionColor);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, c_RefractionWidth, c_RefractionHeight, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void WaterRenderer::draw(Camera &camera, Scene &scene, const ModelManager &models, RendererState &state,
                         RenderSettings &settings) {
    int lastViewportWidth = m_Viewport.width;
    int lastViewportHeight = m_Viewport.height;

    glm::vec4 lastClipPlane = settings.clipPlane;

    // Reflection begin
    camera.setPosition(camera.positionVec() * glm::vec3(1.0, -1.0, 1.0));
    camera.inversePitch();

    settings.clipPlane = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);

    m_Viewport.width = c_ReflectionWidth;
    m_Viewport.height = c_ReflectionHeight;
    m_GRenderer.setGColor(m_ReflectionColor);
    glViewport(0, 0, m_Viewport.width, m_Viewport.height);
    m_GRenderer.resize();
    m_GRenderer.draw(camera, scene, models, state, settings);

    camera.setPosition(camera.positionVec() * glm::vec3(1.0, -1.0, 1.0));
    camera.inversePitch();
    // Reflection end

    // Refraction begin
    settings.clipPlane = glm::vec4(0.0f, -1.0f, 0.0f, 0.0f);

    m_Viewport.width = c_RefractionWidth;
    m_Viewport.height = c_RefractionHeight;
    m_GRenderer.setGColor(m_RefractionColor);
    glViewport(0, 0, m_Viewport.width, m_Viewport.height);
    m_GRenderer.resize();
    m_GRenderer.draw(camera, scene, models, state, settings);
    // Refraction end

    settings.clipPlane = lastClipPlane;

    m_Viewport.width = lastViewportWidth;
    m_Viewport.height = lastViewportHeight;
    glViewport(0, 0, m_Viewport.width, m_Viewport.height);

    m_WaterShader->bind();

    glm::mat4 model(1);
    model = glm::translate(model, glm::vec3(glm::vec3(0.0, 0.0, 0.0)));
    model = glm::scale(model, glm::vec3(10.0));

    m_WaterShader->setFloat3("u_viewPos", camera.positionVec());
    m_WaterShader->setMatrix4("u_view", camera.viewMatrix());
    m_WaterShader->setMatrix4("u_projection", camera.projectionMatrix());
    m_WaterShader->setMatrix4("u_model", model);
    m_WaterShader->setMatrix4("u_noramlFix", glm::rotate(-1.57f, glm::vec3(1.0f, 0.0f, 0.0f)));

    m_WaterShader->setFloat("u_threshold", settings.threshold);

    m_WaterShader->setFloat("u_moveFactor", m_WaterMoveFactor);
    m_WaterMoveFactor += 0.001f;

    auto &light = scene.getDirectedLight();
    glm::vec3 lightDir = glm::quat(light.rotation) * glm::vec3(0.0f, -1.0f, 0.0f);
    m_WaterShader->setFloat3("u_directedLight.direction", lightDir);
    m_WaterShader->setFloat3("u_directedLight.ambient", light.ambient * light.intensity);
    m_WaterShader->setFloat3("u_directedLight.diffuse", light.diffuse * light.intensity);
    m_WaterShader->setFloat3("u_directedLight.specular", light.specular * light.intensity);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_ReflectionColor);
    m_WaterShader->setInt("u_reflectionMap", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_RefractionColor);
    m_WaterShader->setInt("u_refractionMap", 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_GRenderer.gDepth());
    m_WaterShader->setInt("u_depthMap", 2);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, m_GRenderer.gPosition());
    m_WaterShader->setInt("u_positionMap", 3);

    glActiveTexture(GL_TEXTURE4);
    m_WaterDudvMap->bind();
    m_WaterShader->setInt("u_dudvMap", 4);

    glActiveTexture(GL_TEXTURE5);
    m_WaterNormalMap->bind();
    m_WaterShader->setInt("u_normalMap", 5);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindFramebuffer(GL_FRAMEBUFFER, state.fbo);

    glBindVertexArray(m_WaterVBO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 5);
    glBindVertexArray(0);

    glDisable(GL_BLEND);

    m_GRenderer.resetGColor();
    m_GRenderer.resize();
}

} // namespace Engine