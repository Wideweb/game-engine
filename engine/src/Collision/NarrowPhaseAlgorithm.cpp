#include "NarrowPhaseAlgorithm.hpp"

#include <algorithm>
#include <glm/geometric.hpp>
#include <limits>
#include <numeric>

namespace Engine {

glm::vec3
NarrowPhaseAlgorithm::Collide(const std::vector<glm::vec3> &vertices1,
                              const std::vector<glm::vec3> &vertices2) const {
    glm::vec3 mtv;
    float minOverlap = std::numeric_limits<float>::max();

    std::vector<glm::vec3> axis;
    axis.reserve(15);

    axis.push_back(glm::normalize(vertices1[4] - vertices1[0]));
    axis.push_back(glm::normalize(vertices1[3] - vertices1[0]));
    axis.push_back(glm::normalize(vertices1[1] - vertices1[0]));

    axis.push_back(glm::normalize(vertices2[4] - vertices2[0]));
    axis.push_back(glm::normalize(vertices2[3] - vertices2[0]));
    axis.push_back(glm::normalize(vertices2[1] - vertices2[0]));

    std::vector<float> scalars1;
    scalars1.reserve(vertices1.size());

    std::vector<float> scalars2;
    scalars2.reserve(vertices2.size());

    for (size_t i = 0; i < axis.size(); i++) {
        scalars1.clear();
        scalars2.clear();

        for (auto corner : vertices1) {
            scalars1.push_back(glm::dot(axis[i], corner));
        }

        for (auto corner : vertices2) {
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
