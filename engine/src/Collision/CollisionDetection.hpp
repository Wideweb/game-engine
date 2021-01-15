#pragma once

#include "BroadPhaseAlgorithm.hpp"
#include "CollisionShape.hpp"
#include "Math.hpp"
#include "NarrowPhaseAlgorithm.hpp"

#include <glm/vec2.hpp>

namespace Engine {

template <typename T> struct CollisionResult {
    T id;
    glm::vec3 mtv;

    CollisionResult() {}
    CollisionResult(T id, glm::vec3 mtv) : id(id), mtv(std::move(mtv)) {}
};

template <typename T> class CollisionDetection {
  private:
    BroadPhaseAlgorithm<T> m_BroadPhase;
    NarrowPhaseAlgorithm m_NarrowPhase;

  public:
    std::vector<CollisionResult<T>>
    Detect(const std::vector<glm::vec3> &vertices,
           const std::vector<CollisionShape<T>> &shapes) const {

        std::vector<CollisionShape<T>> overlaps =
            m_BroadPhase.ComputeOverlaps(vertices, shapes);

        std::vector<CollisionResult<T>> result;
        result.reserve(overlaps.size());

        for (const auto &overlapedShape : overlaps) {
            glm::vec3 mtv =
                m_NarrowPhase.Collide(vertices, overlapedShape.vertices);
            if (!Math::isEqual(mtv.x + mtv.y + mtv.z, 0.0f)) {
                result.emplace_back(overlapedShape.id, mtv);
            }
        }

        return result;
    }
};

} // namespace Engine