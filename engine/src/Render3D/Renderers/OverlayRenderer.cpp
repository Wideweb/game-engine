#include "OverlayRenderer.hpp"

#include "File.hpp"

#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

namespace Engine {

OverlayRenderer::OverlayRenderer() {
    auto vertexSrc = File::read("./shaders/overlay-vertex-shader.glsl");
    auto fragmentSrc = File::read("./shaders/overlay-fragment-shader.glsl");
    m_Shader = std::make_unique<Shader>(vertexSrc, fragmentSrc);
}

void OverlayRenderer::draw(Camera &camera, Scene &scene, const ModelManager &models) {
    glDisable(GL_DEPTH_TEST);
    m_Shader->bind();

    m_Shader->setMatrix4("u_view", camera.viewMatrix());
    m_Shader->setMatrix4("u_projection", camera.projectionMatrix());

    for (auto &pair : scene.getOverlayObjects()) {
        const auto &model = models.GetModel(pair.first);
        auto &instances = pair.second;
        model->draw(*m_Shader, instances, 0);
    }

    glEnable(GL_DEPTH_TEST);
}

} // namespace Engine