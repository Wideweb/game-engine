#include "AABBOverlap.hpp"
#include "AABB.hpp"

namespace Engine {

bool AABBOverlap::test(const AABB &box1, const AABB &box2) {
    return (box1.max.x > box2.min.x && box1.min.x < box2.max.x &&
            box1.max.y > box2.min.y && box1.min.y < box2.max.y &&
            box1.max.z > box2.min.z && box1.min.z < box2.max.z);
}

} // namespace Engine
