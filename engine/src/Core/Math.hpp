#pragma once

#include <glm/gtx/transform.hpp>
#include <glm/vec3.hpp>

#include <cmath>
#include <cstdlib>
#include <ctime>

namespace Engine::Math {

glm::vec3 getDirection(glm::vec3 rotation);

inline bool isEqual(float x, float y) {
    const float epsilon = 0.00001f;
    return std::abs(x - y) <= epsilon * std::abs(x);
}

inline void srand() { std::srand(static_cast<unsigned>(std::time(0))); }

inline float randFloat() { return static_cast<float>(rand()) / static_cast<float>(RAND_MAX); }

} // namespace Engine::Math