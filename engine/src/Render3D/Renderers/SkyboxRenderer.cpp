#include "SkyboxRenderer.hpp"

#include "File.hpp"

#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

namespace Engine {

SkyboxRenderer::SkyboxRenderer() {
    auto vertexSrc = File::read("./shaders/skybox-vertex-shader.glsl");
    auto fragmentSrc = File::read("./shaders/skybox-fragment-shader.glsl");
    m_SkyboxShader = std::make_unique<Shader>(vertexSrc, fragmentSrc);
}

void SkyboxRenderer::draw(Camera &camera, Scene &scene) {
    if (scene.getSkybox()) {
        // m_SkyboxRotation += 0.01f;

        glDepthFunc(GL_LEQUAL);
        m_SkyboxShader->bind();

        auto fixedView = glm::mat4(glm::mat3(camera.viewMatrix()));
        auto model = glm::rotate(glm::radians(m_SkyboxRotation), glm::vec3(0.0f, 1.0f, 0.0f));

        m_SkyboxShader->setMatrix4("u_model", model);
        m_SkyboxShader->setMatrix4("u_view", fixedView);
        m_SkyboxShader->setMatrix4("u_projection", camera.projectionMatrix());

        scene.getSkybox()->draw(*m_SkyboxShader);

        glDepthFunc(GL_LESS);
    }
}

} // namespace Engine