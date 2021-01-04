#pragma once

#include "CollisionShape.hpp"

#include <vector>

namespace Engine {

class NarrowPhaseAlgorithm {
  public:
    glm::vec2 collide(const CollisionShape &shape1,
                      const CollisionShape &shape2);

  private:
    std::vector<glm::vec2> getAxis(const std::vector<glm::vec2> &corners);
};

} // namespace Engine
