#include "FlareRenderer.hpp"

#include "File.hpp"
#include "TextureLoader.hpp"

#include "glad/glad.h"
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp>

namespace Engine {

FlareRenderer::FlareRenderer(Viewport &viewport, QuadRenderer &quadRenderer)
    : m_Viewport(viewport), m_QuadRenderer(quadRenderer) {
    auto vertexSrc = File::read("./shaders/screen-vertex-shader.glsl");
    auto fragmentSrc = File::read("./shaders/screen-fragment-shader.glsl");
    auto m_LensShader = std::make_unique<Shader>(vertexSrc, fragmentSrc);

    m_LensFlares[0].reset(TextureLoader::loadTexture("./assets/lensFlare/tex6.png"));
    m_LensFlares[1].reset(TextureLoader::loadTexture("./assets/lensFlare/tex4.png"));
    m_LensFlares[2].reset(TextureLoader::loadTexture("./assets/lensFlare/tex2.png"));
    m_LensFlares[3].reset(TextureLoader::loadTexture("./assets/lensFlare/tex7.png"));
    m_LensFlares[4].reset(TextureLoader::loadTexture("./assets/lensFlare/tex3.png"));
    m_LensFlares[5].reset(TextureLoader::loadTexture("./assets/lensFlare/tex5.png"));
    m_LensFlares[6] = m_LensFlares[3];
    m_LensFlares[7] = m_LensFlares[4];
    m_LensFlares[8] = m_LensFlares[5];
    m_LensFlares[9] = m_LensFlares[1];
    m_LensFlares[10].reset(TextureLoader::loadTexture("./assets/lensFlare/tex8.png"));

    m_LensFlareSize[0] = 0.5f;
    m_LensFlareSize[1] = 0.23f;
    m_LensFlareSize[2] = 0.1f;
    m_LensFlareSize[3] = 0.05f;
    m_LensFlareSize[4] = 0.06f;
    m_LensFlareSize[5] = 0.07f;
    m_LensFlareSize[6] = 0.2f;
    m_LensFlareSize[7] = 0.07f;
    m_LensFlareSize[8] = 0.3f;
    m_LensFlareSize[9] = 0.4f;
    m_LensFlareSize[10] = 0.6f;
}

void FlareRenderer::draw(Camera &camera, const glm::vec3 &lightPosition) {
    glm::vec4 sunScreenPos =
        camera.projectionMatrix() * glm::mat4(glm::mat3(camera.viewMatrix())) * glm::vec4(lightPosition, 1.0f);
    float x = (sunScreenPos.x / sunScreenPos.w + 1.0f) / 2.0f;
    float y = (sunScreenPos.y / sunScreenPos.w + 1.0f) / 2.0f;

    glm::vec2 sunToCenter = glm::vec2(0.5f) - glm::vec2(x, y);
    float brightness = (1.0f - glm::length(sunToCenter) / 0.6f) * 0.15f;

    if (brightness > 0 && sunScreenPos.z > 0) {
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        m_LensShader->bind();

        m_LensShader->setFloat("alpha", brightness);

        for (unsigned int i = 0; i < m_LensFlares.size(); i++) {
            glm::vec2 flarePos = glm::vec2(sunScreenPos / sunScreenPos.w) + sunToCenter * (0.4f * i);
            glm::mat4 flareModel = glm::mat4(1.0f);
            flareModel = glm::translate(flareModel, glm::vec3(flarePos, 0.0f));
            flareModel =
                glm::scale(flareModel, glm::vec3(m_LensFlareSize[i],
                                                 m_LensFlareSize[i] * m_Viewport.width / m_Viewport.height, 0.0f));

            m_LensShader->setMatrix4("Model", flareModel);

            glActiveTexture(GL_TEXTURE0);
            m_LensFlares[i]->bind();
            m_LensShader->setInt("colorMap", 0);

            m_QuadRenderer.draw();
        }

        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
    }
}

} // namespace Engine