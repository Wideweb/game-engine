#pragma once

#include "Shader.hpp"
#include "Texture.hpp"

#include "glad/glad.h"
#include <glm/glm.hpp>
#include <memory>

namespace Engine {

struct ParticlesConfiguration {
    glm::vec3 colorFrom = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 colorTo = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 gravity = glm::vec3(0.0, -0.5, 0.0);
    int count = 1000;
    float rate = 0.002f;
    float sizeFrom = 100.0f;
    float sizeTo = 0.0f;
    float lifeTime = 1.0f;
    float spawnRadius = 0.0f;
    float velocityMin = 1.0f;
    float velocityMax = 2.0f;
    float velocityPolarAngle = 1.57f;
    float velocityAzimuthalAngle = 6.28f;
    bool looped = true;

    bool operator==(const ParticlesConfiguration &other) {
        return colorFrom == other.colorFrom && colorTo == other.colorTo && gravity == other.gravity &&
               count == other.count && rate == other.rate && sizeFrom == other.sizeFrom && sizeTo == other.sizeTo &&
               lifeTime == other.lifeTime && spawnRadius == other.spawnRadius && velocityMin == other.velocityMin &&
               velocityMax == other.velocityMax && velocityPolarAngle == other.velocityPolarAngle &&
               velocityAzimuthalAngle == other.velocityAzimuthalAngle && looped == other.looped;
    }

    bool operator!=(const ParticlesConfiguration &other) { return !(*this == other); }
};

class Particles {
  public:
#pragma pack(push, 1)
    struct Particle {
        glm::vec3 startPosition;
        glm::vec3 startVelocity;
        glm::vec3 position;
        glm::vec3 velocity;
        float startTime;
    };
#pragma pack(pop)

    Particles();
    ~Particles();

    void draw(Shader &shader) const;

    void setUp(ParticlesConfiguration config);
    void clear();
    void update(double deltaTime);

    ParticlesConfiguration config();

  private:
    ParticlesConfiguration m_Config;
    bool m_Initialized = false;

    GLuint m_Feedback[2];
    GLuint m_VBO[2];
    GLuint m_VAO[2];

    mutable GLuint m_DrawBuf = 1;

    double m_Time = 0;
    double m_DeltaTime = 0;

    // std::shared_ptr<Texture> m_Texture;
};

} // namespace Engine
