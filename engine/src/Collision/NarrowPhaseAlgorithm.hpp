#pragma once

#include "CollisionShape.hpp"

#include <vector>

namespace Engine {

class NarrowPhaseAlgorithm {
  public:
    glm::vec3 collide(const CollisionShape &shape1,
                      const CollisionShape &shape2) const;
};

} // namespace Engine
