#pragma once

#include <glm/vec3.hpp>

namespace Engine {

class CameraComponent {
  public:
    glm::vec3 offset = glm::vec3(0.0f);
};

} // namespace Engine