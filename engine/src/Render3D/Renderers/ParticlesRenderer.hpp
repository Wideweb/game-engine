#pragma once

#include "Camera.hpp"
#include "Shader.hpp"

#include <glm/glm.hpp>

#include <memory>

namespace Engine {

const size_t c_ParticlesNum = 5000;
const float c_Rate = 0.0003f;

#pragma pack(push, 1)
struct Particle {
    glm::vec3 startPosition;
    glm::vec3 position;
    glm::vec3 velocity;
    float startTime;
};
#pragma pack(pop)

class ParticlesRenderer {
  private:
    std::unique_ptr<Shader> m_Shader;

    unsigned int m_Feedback[2];
    unsigned int m_VBO[2];
    unsigned int m_VAO[2];

    unsigned int m_DrawBuf = 1;

    float m_Time = 0.0f;
    float m_ModelX = 0.0f;
    float m_ModelDX = 0.02f;

  public:
    ParticlesRenderer();

    void draw(Camera &camera);
};

} // namespace Engine