#pragma once

#include "Math.hpp"

namespace Engine::Math {

glm::vec3 getDirection(glm::vec3 rotation) {
    glm::vec3 direction;

    direction.x = cos(rotation.x) * cos(rotation.y);
    direction.y = sin(rotation.x);
    direction.z = cos(rotation.x) * sin(rotation.y);
    direction = glm::normalize(direction);

    return direction;
}

} // namespace Engine::Math