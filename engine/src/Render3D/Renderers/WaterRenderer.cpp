#include "WaterRenderer.hpp"

#include "File.hpp"
#include "TextureLoader.hpp"

#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp>

namespace Engine {

WaterRenderer::WaterRenderer(GRenderer &gRenderer) : m_GRenderer(gRenderer) {
    auto vertexSrc = File::read("./shaders/water-vertex-shader.glsl");
    auto fragmentSrc = File::read("./shaders/water-fragment-shader.glsl");
    m_WaterShader = std::make_unique<Shader>(vertexSrc, fragmentSrc);

    m_WaterDudvMap.reset(TextureLoader::loadTextureRGB("./shaders/waterDUDV.png"));
    m_WaterNormalMap.reset(TextureLoader::loadTextureRGB("./shaders/waterNormalMap.png"));

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
}

void WaterRenderer::draw(Camera &camera, Scene &scene, const ModelManager &models, RendererState &state) {
    camera.setPosition(camera.positionVec() * glm::vec3(1.0, -1.0, 1.0));
    camera.inversePitch();

    m_GRenderer.draw(camera, scene, models, state);

    camera.setPosition(camera.positionVec() * glm::vec3(1.0, -1.0, 1.0));
    camera.inversePitch();

    m_WaterShader->bind();

    glm::mat4 model(1);
    model = glm::translate(model, glm::vec3(glm::vec3(10.0, 0.0, 10.0)));
    model = glm::scale(model, glm::vec3(10.0));

    m_WaterShader->setFloat3("u_viewPos", camera.positionVec());
    m_WaterShader->setMatrix4("u_view", camera.viewMatrix());
    m_WaterShader->setMatrix4("u_projection", camera.projectionMatrix());
    m_WaterShader->setMatrix4("u_model", model);

    m_WaterShader->setFloat("u_moveFactor", m_WaterMoveFactor);
    m_WaterMoveFactor += 0.001f;

    m_WaterShader->setFloat3("u_lightDir", scene.getDirectedLight().direction);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_GRenderer.gColor());
    m_WaterShader->setInt("u_colorMap", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_GRenderer.gDepth());
    m_WaterShader->setInt("u_depthMap", 1);

    glActiveTexture(GL_TEXTURE2);
    m_WaterDudvMap->bind();
    m_WaterShader->setInt("u_dudvMap", 2);

    glActiveTexture(GL_TEXTURE3);
    m_WaterNormalMap->bind();
    m_WaterShader->setInt("u_normalMap", 3);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindFramebuffer(GL_FRAMEBUFFER, state.fbo);

    glBindVertexArray(m_WaterVBO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);

    glDisable(GL_BLEND);
}

} // namespace Engine