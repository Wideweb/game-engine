#pragma once

#include "AABB.hpp"

namespace Engine {

class AABBOverlap {
  public:
    static bool test(AABB &a, AABB &b);
};

} // namespace Engine
