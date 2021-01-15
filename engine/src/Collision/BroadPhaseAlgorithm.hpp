#pragma once

#include "AABB.hpp"
#include "AABBOverlap.hpp"
#include "CollisionShape.hpp"

#include <vector>

namespace Engine {

template <typename T> class BroadPhaseAlgorithm {
  public:
    std::vector<CollisionShape<T>>
    ComputeOverlaps(const std::vector<glm::vec3> &vertices,
                    const std::vector<CollisionShape<T>> &shapes) const {
        std::vector<CollisionShape<T>> overlaps;
        overlaps.reserve(shapes.size());

        for (size_t i = 0; i < shapes.size(); i++) {
            if (AABBOverlap::test(AABB(vertices), AABB(shapes[i].vertices))) {
                overlaps.push_back(shapes[i]);
            }
        }

        return overlaps;
    }
};

} // namespace Engine
