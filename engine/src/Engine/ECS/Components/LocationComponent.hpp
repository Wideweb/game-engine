#pragma once

#include "Math.hpp"

#include <glm/vec3.hpp>

namespace Engine {

class LocationComponent {
  public:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 front;
    bool updated = false;

    LocationComponent() {}
    LocationComponent(glm::vec3 position, glm::vec3 rotation)
        : position(position), rotation(rotation),
          front(Math::getDirection(rotation)) {}
};

} // namespace Engine