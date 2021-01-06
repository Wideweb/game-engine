#pragma once

#include "BroadPhaseAlgorithm.hpp"
#include "CollisionShape.hpp"
#include "NarrowPhaseAlgorithm.hpp"

#include <glm/vec2.hpp>

namespace Engine {

struct CollisionResult {
    uint32_t shape1;
    uint32_t shape2;
    glm::vec3 mtv;

    CollisionResult() {}
    CollisionResult(uint32_t shape1, uint32_t shape2, glm::vec3 mtv)
        : shape1(shape1), shape2(shape2), mtv(std::move(mtv)) {}
};

class CollisionDetection {
  private:
    BroadPhaseAlgorithm m_BroadPhase;
    NarrowPhaseAlgorithm m_NarrowPhase;

  public:
    std::vector<CollisionResult>
    detect(const std::vector<CollisionShape> &shapesd) const;
};

} // namespace Engine