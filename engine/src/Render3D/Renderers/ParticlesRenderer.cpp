#include "ParticlesRenderer.hpp"

#include "File.hpp"

#include "glad/glad.h"
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp>

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>

namespace Engine {

float randFloat() { return static_cast<float>(rand()) / static_cast<float>(RAND_MAX); }

ParticlesRenderer::ParticlesRenderer() {
    auto vertexSrc = File::read("./shaders/particles-vertex-shader.glsl");
    auto fragmentSrc = File::read("./shaders/particles-fragment-shader.glsl");

    std::vector<std::string> transformFeedbackVaryings = {"v_startPosition", "v_position", "v_velocity", "v_startTime"};
    m_Shader = std::make_unique<Shader>(vertexSrc, fragmentSrc, transformFeedbackVaryings);

    std::srand(static_cast<unsigned>(std::time(0)));

    Particle particles[c_ParticlesNum];

    for (unsigned int i = 0; i < c_ParticlesNum; i++) {
        float phi = glm::mix(0.0f, 6.28f, randFloat());
        float offset = glm::mix(0.0f, 1.0f, randFloat());

        particles[i].position.x = std::cosf(phi) * offset;
        particles[i].position.z = std::sinf(phi) * offset;
        particles[i].position.y = 0.0f;

        particles[i].startPosition = particles[i].position;
    }

    for (unsigned int i = 0; i < c_ParticlesNum; i++) {
        glm::vec3 &position = particles[i].position;

        float theta = glm::mix(0.0f, 3.14f / 20.0f, randFloat());
        float phi = glm::mix(0.0f, 6.28f, randFloat());
        float distanceToCenter = glm::distance(glm::vec2(0.0f), glm::vec2(position.x, position.z));
        float velocityFactor = glm::mix(1.0f, 2.0f, randFloat()) * (1.0f - distanceToCenter);

        glm::vec3 &velocity = particles[i].velocity;

        velocity.x = std::sinf(theta) * std::cosf(phi);
        velocity.y = std::cosf(theta);
        velocity.z = std::sinf(theta) * std::sinf(phi);

        velocity = velocity * velocityFactor;
    }

    float time = 0.0f;
    for (unsigned int i = 0; i < c_ParticlesNum; i++) {
        particles[i].startTime = time;
        time += c_Rate;
    }

    for (unsigned int i = 0; i < 2; i++) {
        glGenVertexArrays(1, &m_VAO[i]);
        glBindVertexArray(m_VAO[i]);

        glGenBuffers(1, &m_VBO[i]);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(particles), &particles, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), reinterpret_cast<void *>(0));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), reinterpret_cast<void *>(3 * sizeof(float)));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), reinterpret_cast<void *>(6 * sizeof(float)));

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), reinterpret_cast<void *>(9 * sizeof(float)));
    }

    glGenTransformFeedbacks(2, m_Feedback);

    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_Feedback[0]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_VBO[0]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, m_VBO[0]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, m_VBO[0]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 3, m_VBO[0]);

    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_Feedback[1]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_VBO[1]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, m_VBO[1]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 3, m_VBO[1]);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void ParticlesRenderer::draw(Camera &camera) {
    // VERTEX_PROGRAM_POINT_SIZE
    glEnable(0x8642);

    m_Shader->bind();

    glm::mat4 model(1);
    model = glm::translate(model, glm::vec3(m_ModelX, 1.0f, 0.0f));

    m_ModelX += m_ModelDX;

    if (m_ModelX > 10) {
        m_ModelDX = -0.02f;
    }

    if (m_ModelX < 0) {
        m_ModelDX = 0.02f;
    }

    m_Shader->setMatrix4("u_view", camera.viewMatrix());
    m_Shader->setMatrix4("u_projection", camera.projectionMatrix());
    m_Shader->setFloat3("u_viewPos", camera.positionVec());
    m_Shader->setMatrix4("u_model", model);

    float deltaTime = 0.033f;
    m_Time += deltaTime;
    m_Shader->setFloat("u_time", m_Time);
    m_Shader->setFloat("u_deltaTime", deltaTime);
    m_Shader->setFloat("u_lifetime", c_Rate * c_ParticlesNum);

    // glEnable(GL_RASTERIZER_DISCARD);

    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_Feedback[m_DrawBuf]);

    glBeginTransformFeedback(GL_POINTS);
    glBindVertexArray(m_VAO[1 - m_DrawBuf]);
    glDrawArrays(GL_POINTS, 0, c_ParticlesNum);
    glEndTransformFeedback();

    // glDisable(GL_RASTERIZER_DISCARD);

    // glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &renderSub);
    // glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(m_VAO[m_DrawBuf]);
    glDrawArrays(GL_POINTS, 0, c_ParticlesNum);
    // glDrawTransformFeedback(GL_POINTS, m_Feedback[m_DrawBuf]);

    glBindVertexArray(0);

    m_DrawBuf = 1 - m_DrawBuf;
}

} // namespace Engine