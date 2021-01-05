#pragma once

#include <glm/gtx/transform.hpp>
#include <glm/vec3.hpp>

namespace Engine::Math {

glm::vec3 getDirection(glm::vec3 rotation);

inline bool isEqual(float x, float y) {
    const float epsilon = 0.00001f;
    return std::abs(x - y) <= epsilon * std::abs(x);
}

} // namespace Engine::Math