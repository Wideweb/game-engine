#include "NarrowPhaseAlgorithm.hpp"

#include <algorithm>
#include <glm/geometric.hpp>
#include <limits>
#include <numeric>

namespace Engine {

glm::vec3 NarrowPhaseAlgorithm::collide(const CollisionShape &shape1,
                                        const CollisionShape &shape2) const {
    glm::vec3 mtv;
    float minOverlap = std::numeric_limits<float>::max();

    std::vector<glm::vec3> axis;
    axis.reserve(15);

    axis.push_back(glm::normalize(shape1.vertices[4] - shape1.vertices[0]));
    axis.push_back(glm::normalize(shape1.vertices[3] - shape1.vertices[0]));
    axis.push_back(glm::normalize(shape1.vertices[1] - shape1.vertices[0]));

    axis.push_back(glm::normalize(shape2.vertices[4] - shape2.vertices[0]));
    axis.push_back(glm::normalize(shape2.vertices[3] - shape2.vertices[0]));
    axis.push_back(glm::normalize(shape2.vertices[1] - shape2.vertices[0]));

    std::vector<float> scalars1;
    scalars1.reserve(shape1.vertices.size());

    std::vector<float> scalars2;
    scalars2.reserve(shape2.vertices.size());

    for (size_t i = 0; i < axis.size(); i++) {
        scalars1.clear();
        scalars2.clear();

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
            return glm::vec3(0);
        }

        float overlap = s1max > s2max ? (s2max - s1min) : (s2min - s1max);
        if (minOverlap > std::abs(overlap)) {
            minOverlap = std::abs(overlap);
            mtv = axis[i] * overlap;
        }
    }

    return mtv;
}

} // namespace Engine
