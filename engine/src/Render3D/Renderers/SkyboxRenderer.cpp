#include "SkyboxRenderer.hpp"

#include "File.hpp"
#include "GLSLPreprocessor.hpp"

#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

namespace Engine {

SkyboxRenderer::SkyboxRenderer() {
    auto vertexSrc = GLSLPreprocessor::preprocess("./shaders/pass/skybox.vertex.glsl").sourceCode;
    auto fragmentSrc = GLSLPreprocessor::preprocess("./shaders/pass/skybox.fragment.glsl").sourceCode;
    m_Shader = Shader(vertexSrc, fragmentSrc);
}

SkyboxRenderer::~SkyboxRenderer() { m_Shader.free(); }

void SkyboxRenderer::draw(Camera &camera, Scene &scene, RenderSettings &settings) {
    if (scene.getSkybox()) {
        const auto &skybox = scene.getSkybox();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glDepthFunc(GL_LEQUAL);
        m_Shader.bind();

        auto fixedView = glm::mat4(glm::mat3(camera.viewMatrix()));
        auto model = glm::rotate(skybox->rotation, glm::vec3(0.0f, 1.0f, 0.0f));

        m_Shader.setMatrix4("u_model", model);
        m_Shader.setMatrix4("u_view", fixedView);
        m_Shader.setMatrix4("u_projection", camera.projectionMatrix());
        m_Shader.setFloat4("u_clipPlane", settings.clipPlane);
        m_Shader.setInt("u_fog", settings.fog);
        m_Shader.setFloat3("u_fogColor", settings.fogColor);
        m_Shader.setFloat("u_threshold", settings.threshold);
        m_Shader.setFloat("u_lowerLimit", skybox->lowerLimit);
        m_Shader.setFloat("u_upperLimit", skybox->upperLimit);

        skybox->draw(m_Shader);

        glDisable(GL_BLEND);
        glDepthFunc(GL_LESS);

        skybox->rotation += skybox->rotationSpeed;
    }
}

} // namespace Engine