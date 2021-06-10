#pragma once

#include "Camera.hpp"
#include "Particles.hpp"
#include "Shader.hpp"

#include <glm/glm.hpp>

#include <memory>

namespace Engine {

class ParticlesRenderer {
  private:
    std::unique_ptr<Shader> m_Shader;

  public:
    ParticlesRenderer();

    void draw(const Particles &particles, const glm::mat4 &position, Camera &camera);
};

} // namespace Engine