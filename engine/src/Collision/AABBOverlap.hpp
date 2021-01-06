#pragma once

#include "AABB.hpp"

namespace Engine {

class AABBOverlap {
  public:
    static bool test(const AABB &a, const AABB &b);
};

} // namespace Engine
