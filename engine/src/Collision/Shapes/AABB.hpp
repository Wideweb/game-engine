#pragma once

#include "Math.hpp"

#include <algorithm>
#include <glm/vec3.hpp>
#include <vector>

namespace Engine {

struct AABB {
    glm::vec3 min;
    glm::vec3 max;

    AABB() {}
    AABB(glm::vec3 min, glm::vec3 max) : min(std::move(min)), max(std::move(max)) {}
    AABB(const std::vector<glm::vec3> &vertices) {
        std::vector<float> coords;
        coords.reserve(vertices.size());

        std::transform(vertices.begin(), vertices.end(), std::back_inserter(coords), [](glm::vec3 v) { return v.x; });

        max.x = *std::max_element(coords.begin(), coords.end());
        min.x = *std::min_element(coords.begin(), coords.end());

        if (max.x == min.x) {
            max.x += Math::c_Epsilon;
        }

        coords.clear();

        std::transform(vertices.begin(), vertices.end(), std::back_inserter(coords), [](glm::vec3 v) { return v.y; });

        max.y = *std::max_element(coords.begin(), coords.end());
        min.y = *std::min_element(coords.begin(), coords.end());

        if (max.y == min.y) {
            max.y += Math::c_Epsilon;
        }

        coords.clear();

        std::transform(vertices.begin(), vertices.end(), std::back_inserter(coords), [](glm::vec3 v) { return v.z; });

        max.z = *std::max_element(coords.begin(), coords.end());
        min.z = *std::min_element(coords.begin(), coords.end());

        if (max.z == min.z) {
            max.z += Math::c_Epsilon;
        }
    }
};

} // namespace Engine
