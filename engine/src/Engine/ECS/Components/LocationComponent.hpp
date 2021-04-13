#pragma once

#include "Math.hpp"

#include <glm/vec3.hpp>

namespace Engine {

class LocationComponent {
  public:
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec3 front = glm::vec3(0.0f);
    bool updated = false;

    LocationComponent() {}

    LocationComponent(float x, float y, float z)
        : LocationComponent(glm::vec3(x, y, z)) {}

    LocationComponent(glm::vec3 position)
        : LocationComponent(position, glm::vec3(0)) {}

    LocationComponent(glm::vec3 position, glm::vec3 rotation)
        : position(position), rotation(rotation),
          front(Math::getDirection(rotation)) {}
};

} // namespace Engine