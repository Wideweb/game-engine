#include "CollisionDetection.hpp"
#include "Math.hpp"

namespace Engine {

std::vector<CollisionResult>
CollisionDetection::detect(const std::vector<CollisionShape> &shapes) const {
    std::vector<OverlappingPair> pairs =
        m_BroadPhase.computeOverlappingPairs(shapes);

    std::vector<CollisionResult> result;
    result.reserve(pairs.size());

    for (const auto &pair : pairs) {
        glm::vec3 mtv = m_NarrowPhase.collide(pair.shape1, pair.shape2);
        if (!Math::isEqual(mtv.x + mtv.y + mtv.z, 0.0f)) {
            result.emplace_back(pair.shape1.id, pair.shape2.id, mtv);
        }
    }

    return result;
}

} // namespace Engine