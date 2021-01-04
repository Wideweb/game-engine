#include "AABBOverlap.hpp"
#include "AABB.hpp"

namespace Engine {

bool AABBOverlap::test(AABB &a, AABB &b) {
    float d1x = b.min.x - a.max.x;
    float d1y = b.min.y - a.max.y;
    float d2x = a.min.x - b.max.x;
    float d2y = a.min.y - b.max.y;

    if (d1x > 0.0f || d1y > 0.0f)
        return false;

    if (d2x > 0.0f || d2y > 0.0f)
        return false;

    return true;
}

} // namespace Engine
