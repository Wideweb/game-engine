#include "BroadPhaseAlgorithm.hpp"
#include "AABBOverlap.hpp"

namespace Engine {

std::vector<OverlappingPair> BroadPhaseAlgorithm::computeOverlappingPairs(
    const std::vector<CollisionShape> &shapes) const {
    std::vector<OverlappingPair> pairs;

    std::vector<AABB> aabbs;
    std::transform(
        shapes.begin(), shapes.end(), std::back_inserter(aabbs),
        [](const CollisionShape &shape) { return AABB(shape.vertices); });

    for (size_t i = 0; i < shapes.size() - 1; i++) {
        for (size_t j = i + 1; j < shapes.size(); j++) {
            if (shapes[i].isStatic && shapes[j].isStatic) {
                continue;
            }

            if (AABBOverlap::test(aabbs[i], aabbs[j])) {
                if (shapes[i].isStatic) {
                    pairs.emplace_back(shapes[j], shapes[i]);
                } else {
                    pairs.emplace_back(shapes[i], shapes[j]);
                }
            }
        }
    }

    return pairs;
}

} // namespace Engine
