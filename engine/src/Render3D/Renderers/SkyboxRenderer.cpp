#include "SkyboxRenderer.hpp"

#include "File.hpp"

#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

namespace Engine {

SkyboxRenderer::SkyboxRenderer() {
    auto vertexSrc = File::read("./shaders/skybox-vertex-shader.glsl");
    auto fragmentSrc = File::read("./shaders/skybox-fragment-shader.glsl");
    m_Shader = Shader(vertexSrc, fragmentSrc);
}

SkyboxRenderer::~SkyboxRenderer() { m_Shader.free(); }

void SkyboxRenderer::draw(Camera &camera, Scene &scene, RenderSettings &settings) {
    if (scene.getSkybox()) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glDepthFunc(GL_LEQUAL);
        m_Shader.bind();

        auto fixedView = glm::mat4(glm::mat3(camera.viewMatrix()));
        auto model = glm::rotate(scene.getSkybox()->rotation, glm::vec3(0.0f, 1.0f, 0.0f));

        m_Shader.setMatrix4("u_model", model);
        m_Shader.setMatrix4("u_view", fixedView);
        m_Shader.setMatrix4("u_projection", camera.projectionMatrix());
        m_Shader.setFloat4("u_clipPlane", settings.clipPlane);

        m_Shader.setFloat("u_threshold", settings.threshold);

        scene.getSkybox()->draw(m_Shader);

        glDisable(GL_BLEND);
        glDepthFunc(GL_LESS);

        scene.getSkybox()->rotation += scene.getSkybox()->rotationSpeed;
    }
}

} // namespace Engine