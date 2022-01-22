#pragma once

#include <glm/gtx/transform.hpp>
#include <glm/vec3.hpp>

#include <cmath>
#include <cstdlib>
#include <ctime>

namespace Engine::Math {

const float c_Epsilon = 0.00001f;

glm::vec3 getDirection(glm::vec3 rotation);

inline bool isEqual(float x, float y) {
    const float epsilon = c_Epsilon;
    return std::abs(x - y) <= epsilon * std::abs(x);
}

inline void srand() { std::srand(static_cast<unsigned>(std::time(0))); }

inline float randFloat() { return static_cast<float>(rand()) / static_cast<float>(RAND_MAX); }

inline float rescale(float value, float prevScale, float newScale) {
    if (isEqual(prevScale, 0.0f) || isEqual(value, 0.0f)) {
        return newScale;
    }

    return value / prevScale * newScale;
}

inline glm::vec3 rescale(glm::vec3 value, glm::vec3 prevScale, glm::vec3 newScale) {
    return glm::vec3(rescale(value.x, prevScale.x, newScale.x), rescale(value.y, prevScale.y, newScale.y),
                     rescale(value.z, prevScale.z, newScale.z));
}

} // namespace Engine::Math