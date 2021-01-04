#pragma once

#include "Entity.hpp"

#include <glm/vec3.hpp>

namespace Engine {

class VelocityComponent {
  public:
    float speed;
    glm::vec3 rotation;

    VelocityComponent() {}
    VelocityComponent(float speed, glm::vec3 rotation)
        : speed(speed), rotation(rotation) {}
};

} // namespace Engine