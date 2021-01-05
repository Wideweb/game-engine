#include "CollisionDetection.hpp"
#include "Math.hpp"

namespace Engine {

std::vector<CollisionResult>
CollisionDetection::detect(const std::vector<CollisionShape> &shapes) const {
    std::vector<CollisionResult> result;

    std::vector<OverlappingPair> pairs =
        m_BroadPhase.computeOverlappingPairs(shapes);

    for (const auto &pair : pairs) {
        glm::vec2 mtv = m_NarrowPhase.collide(pair.shape1, pair.shape2);
        if (!Math::isEqual(mtv.x, 0.0f) || !Math::isEqual(mtv.y, 0.0f)) {
            result.emplace_back(pair.shape1.id, pair.shape2.id, mtv);
        }
    }

    return result;
}

} // namespace Engine