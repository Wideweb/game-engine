#pragma once

#include "CollisionShape.hpp"

#include <vector>

namespace Engine {

class NarrowPhaseAlgorithm {
  public:
    glm::vec3 Collide(const std::vector<glm::vec3> &vertices1,
                      const std::vector<glm::vec3> &vertices2,
                      const glm::vec3 &offset1) const;
};

} // namespace Engine
