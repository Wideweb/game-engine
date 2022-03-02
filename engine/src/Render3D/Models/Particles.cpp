#include "Particles.hpp"

#include "Math.hpp"

#include "glad/glad.h"

#include <glm/gtx/transform.hpp>
#include <vector>

namespace Engine {

Particles::Particles() {}
Particles::~Particles() {}

ParticlesConfiguration Particles::config() { return m_Config; }

void Particles::setUp(ParticlesConfiguration config) {
    m_Config = config;

    // m_Texture.reset(TextureLoader::loadTexture("./assets/particles/red.png"));

    std::vector<Particle> particles;
    particles.resize(m_Config.count);

    for (unsigned int i = 0; i < m_Config.count; i++) {
        float phi = glm::mix(0.0f, 6.28f, Math::randFloat());
        float r = glm::mix(0.0f, m_Config.spawnRadius, Math::randFloat());

        particles[i].position.x = std::cosf(phi) * r;
        particles[i].position.z = std::sinf(phi) * r;
        particles[i].position.y = 0.0f;

        particles[i].startPosition = particles[i].position;
    }

    for (unsigned int i = 0; i < m_Config.count; i++) {
        float theta = glm::mix(0.0f, m_Config.velocityPolarAngle, Math::randFloat());
        float phi = glm::mix(0.0f, m_Config.velocityAzimuthalAngle, Math::randFloat());
        float velocityR = glm::mix(m_Config.velocityMin, m_Config.velocityMax, Math::randFloat());

        glm::vec3 &velocity = particles[i].velocity;

        velocity.x = std::sinf(theta) * std::cosf(phi);
        velocity.y = std::cosf(theta);
        velocity.z = std::sinf(theta) * std::sinf(phi);

        velocity = velocity * velocityR;
        particles[i].startVelocity = velocity;
    }

    float time = 0.0f;
    for (unsigned int i = 0; i < m_Config.count; i++) {
        particles[i].startTime = time;
        time += m_Config.rate;
    }

    for (unsigned int i = 0; i < 2; i++) {
        if (!m_Initialized) {
            glGenVertexArrays(1, &m_VAO[i]);
        }
        glBindVertexArray(m_VAO[i]);

        if (!m_Initialized) {
            glGenBuffers(1, &m_VBO[i]);
        }
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO[i]);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(Particle) * particles.size()), particles.data(),
                     GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), reinterpret_cast<void *>(0));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), reinterpret_cast<void *>(3 * sizeof(float)));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), reinterpret_cast<void *>(6 * sizeof(float)));

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), reinterpret_cast<void *>(9 * sizeof(float)));

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), reinterpret_cast<void *>(12 * sizeof(float)));
    }

    if (!m_Initialized) {
        glGenTransformFeedbacks(2, m_Feedback);
    }

    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_Feedback[0]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_VBO[0]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, m_VBO[0]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, m_VBO[0]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 3, m_VBO[0]);

    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_Feedback[1]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_VBO[1]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, m_VBO[1]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, m_VBO[1]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 3, m_VBO[1]);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    m_Initialized = true;
}

void Particles::update(double deltaTime) {
    m_DeltaTime = deltaTime;
    m_Time += deltaTime;
}

void Particles::clear() {
    glDeleteVertexArrays(2, m_VAO);
    glDeleteBuffers(2, m_VBO);
    glDeleteTransformFeedbacks(2, m_Feedback);
    m_Initialized = false;
}

void Particles::draw(Shader &shader) const {
    shader.setFloat("u_time", static_cast<float>(m_Time));
    shader.setFloat("u_deltaTime", static_cast<float>(m_DeltaTime));

    shader.setFloat("u_lifetime", m_Config.lifeTime);
    shader.setFloat("u_sizeFrom", m_Config.sizeFrom);
    shader.setFloat("u_sizeTo", m_Config.sizeTo);
    shader.setFloat3("u_colorFrom", m_Config.colorFrom);
    shader.setFloat3("u_colorTo", m_Config.colorTo);
    shader.setFloat3("u_gravity", m_Config.gravity);

    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_Feedback[m_DrawBuf]);

    // discard
    shader.setInt("u_draw", 0);
    glBeginTransformFeedback(GL_POINTS);
    glBindVertexArray(m_VAO[1 - m_DrawBuf]);
    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(m_Config.count));
    glEndTransformFeedback();

    shader.setInt("u_draw", 1);

    // glActiveTexture(GL_TEXTURE0);
    // m_Texture->bind();
    // shader.setInt("u_colorMap", 0);

    glBindVertexArray(m_VAO[m_DrawBuf]);
    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(m_Config.count));

    glBindVertexArray(0);

    m_DrawBuf = 1 - m_DrawBuf;
}

} // namespace Engine
