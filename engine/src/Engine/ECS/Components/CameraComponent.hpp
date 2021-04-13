#pragma once

#include <glm/vec3.hpp>

namespace Engine {

class CameraComponent {
  public:
    glm::vec3 offset = glm::vec3(0.0f);

    CameraComponent() {}
    CameraComponent(float x, float y, float z) : offset(glm::vec3(x, y, z)) {}
    CameraComponent(glm::vec3 offset) : offset(offset) {}
};

} // namespace Engine