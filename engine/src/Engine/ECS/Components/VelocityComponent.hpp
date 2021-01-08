#pragma once

#include "Entity.hpp"

#include <glm/vec3.hpp>

namespace Engine {

class VelocityComponent {
  public:
    float speed = 0.0f;
    glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec3 velocity = glm::vec3(0.0f);

    VelocityComponent() {}
    VelocityComponent(float speed, glm::vec3 rotation, glm::vec3 velocity)
        : speed(speed), rotation(std::move(rotation)),
          velocity(std::move(velocity)) {}
};

} // namespace Engine