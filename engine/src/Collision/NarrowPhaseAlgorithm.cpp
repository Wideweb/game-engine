#include "NarrowPhaseAlgorithm.hpp"

#include <algorithm>
#include <glm/geometric.hpp>
#include <limits>
#include <numeric>

namespace Engine {

glm::vec2 NarrowPhaseAlgorithm::collide(const CollisionShape &shape1,
                                        const CollisionShape &shape2) const {
    glm::vec2 mtv;
    float minOverlap = std::numeric_limits<float>::max();

    auto axis1 = getAxis(shape1.vertices);
    auto axis2 = getAxis(shape2.vertices);

    std::vector<glm::vec2> axis;

    axis.insert(axis.end(), axis1.begin(), axis1.end());
    axis.insert(axis.end(), axis2.begin(), axis2.end());

    for (size_t i = 0; i < axis.size(); i++) {
        std::vector<float> scalars1;
        std::vector<float> scalars2;

        for (auto corner : shape1.vertices) {
            scalars1.push_back(glm::dot(axis[i], corner));
        }

        for (auto corner : shape2.vertices) {
            scalars2.push_back(glm::dot(axis[i], corner));
        }

        float s1max = *std::max_element(scalars1.begin(), scalars1.end());
        float s1min = *std::min_element(scalars1.begin(), scalars1.end());

        float s2max = *std::max_element(scalars2.begin(), scalars2.end());
        float s2min = *std::min_element(scalars2.begin(), scalars2.end());

        if (s1min > s2max || s2min > s1max) {
            return glm::vec2(0);
        }

        float overlap = s1max > s2max ? (s2max - s1min) : (s2min - s1max);
        if (minOverlap > std::abs(overlap)) {
            minOverlap = std::abs(overlap);
            mtv = axis[i] * overlap;
        }
    }

    return mtv;
}

std::vector<glm::vec2>
NarrowPhaseAlgorithm::getAxis(const std::vector<glm::vec2> &corners) const {
    std::vector<glm::vec2> axis;
    for (size_t i = 0; i < corners.size() - 1; i++) {
        glm::vec2 v1 = corners[i];
        glm::vec2 v2 = corners[i + 1];
        glm::vec2 d = v2 - v1;

        glm::vec2 perp(d.y, -d.x);
        glm::normalize(perp);

        axis.push_back(glm::normalize(perp));
    }

    glm::vec2 v1 = corners.back();
    glm::vec2 v2 = corners.front();
    glm::vec2 d = v2 - v1;

    glm::vec2 perp(d.y, -d.x);
    glm::normalize(perp);

    axis.push_back(glm::normalize(perp));

    return axis;
}

} // namespace Engine
