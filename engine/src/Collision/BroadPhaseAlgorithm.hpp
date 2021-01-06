#pragma once

#include "AABB.hpp"
#include "CollisionShape.hpp"

#include <vector>

namespace Engine {

struct OverlappingPair {
    CollisionShape shape1;
    CollisionShape shape2;

    OverlappingPair() {}
    OverlappingPair(CollisionShape shape1, CollisionShape shape2)
        : shape1(std::move(shape1)), shape2(std::move(shape2)) {}
};

class BroadPhaseAlgorithm {
  public:
    std::vector<OverlappingPair>
    computeOverlappingPairs(const std::vector<CollisionShape> &shapes) const;
};

} // namespace Engine
