#include "ParticlesRenderer.hpp"

#include "File.hpp"

#include "glad/glad.h"

#include <glm/mat4x4.hpp>

#include <string>
#include <vector>

namespace Engine {

ParticlesRenderer::ParticlesRenderer() {
    auto vertexSrc = File::readGLSL("./shaders/pass/particles.vertex.glsl");
    auto fragmentSrc = File::readGLSL("./shaders/pass/particles.fragment.glsl");

    std::vector<std::string> transformFeedbackVaryings = {"v_startPosition", "v_startVelocity", "v_position",
                                                          "v_velocity", "v_startTime"};
    m_Shader = Shader(vertexSrc, fragmentSrc, transformFeedbackVaryings);
}

ParticlesRenderer::~ParticlesRenderer() { m_Shader.free(); }

void ParticlesRenderer::draw(const Particles &particles, const glm::mat4 &model, Camera &camera,
                             RenderSettings &settings) {
    m_Shader.bind();
    m_Shader.setMatrix4("u_view", camera.viewMatrix());
    m_Shader.setMatrix4("u_projection", camera.projectionMatrix());
    m_Shader.setFloat3("u_viewPos", camera.positionVec());
    m_Shader.setMatrix4("u_model", model);
    m_Shader.setFloat("u_threshold", settings.threshold);
    m_Shader.setInt("u_fog", settings.fog);
    m_Shader.setFloat3("u_fogColor", settings.fogColor);
    m_Shader.setFloat("u_density", settings.fogDensity);
    m_Shader.setFloat("u_gradient", settings.fogGradient);

    // VERTEX_PROGRAM_POINT_SIZE
    glEnable(0x8642);

    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // glDepthFunc(GL_ALWAYS);

    particles.draw(m_Shader);

    // glDepthFunc(GL_LESS);

    // glDisable(GL_BLEND);
}

} // namespace Engine