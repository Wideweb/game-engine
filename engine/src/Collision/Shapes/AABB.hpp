#pragma once

#include <algorithm>
#include <glm/vec2.hpp>
#include <vector>

namespace Engine {

struct AABB {
    glm::vec2 min;
    glm::vec2 max;

    AABB() {}
    AABB(const std::vector<glm::vec2> &vertices) {
        std::vector<float> x;
        std::transform(vertices.begin(), vertices.end(), std::back_inserter(x),
                       [](glm::vec2 v) { return v.x; });

        std::vector<float> y;
        std::transform(vertices.begin(), vertices.end(), std::back_inserter(y),
                       [](glm::vec2 v) { return v.y; });

        max.x = *std::max_element(x.begin(), x.end());
        max.y = *std::max_element(y.begin(), y.end());

        min.x = *std::min_element(x.begin(), x.end());
        min.y = *std::min_element(y.begin(), y.end());
    }
};

} // namespace Engine
