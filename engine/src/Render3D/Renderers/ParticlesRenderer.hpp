#pragma once

#include "Camera.hpp"
#include "Particles.hpp"
#include "RenderSettings.hpp"
#include "Shader.hpp"

#include <glm/glm.hpp>

namespace Engine {

class ParticlesRenderer {
  private:
    Shader m_Shader;

  public:
    ParticlesRenderer();
    ~ParticlesRenderer();

    void draw(const Particles &particles, const glm::mat4 &position, Camera &camera, RenderSettings &settings);
};

} // namespace Engine