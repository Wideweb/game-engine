#pragma once

#include "AABB.hpp"

#include <glm/vec3.hpp>

namespace Engine {

class AABBOverlap {
  public:
    static bool test(const AABB &a, const AABB &b);
    static bool testSegment(glm::vec3 p0, glm::vec3 p1, const AABB &box);
};

} // namespace Engine
