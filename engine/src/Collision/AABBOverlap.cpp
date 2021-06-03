#include "AABBOverlap.hpp"
#include "AABB.hpp"

#include <cmath>

namespace Engine {

bool AABBOverlap::test(const AABB &box1, const AABB &box2) {
    return (box1.max.x > box2.min.x && box1.min.x < box2.max.x && box1.max.y > box2.min.y && box1.min.y < box2.max.y &&
            box1.max.z > box2.min.z && box1.min.z < box2.max.z);
}

bool AABBOverlap::testSegment(glm::vec3 p0, glm::vec3 p1, const AABB &box) {
    const float EPSILON = 0.00001f;

    glm::vec3 boxCenter = (box.min + box.max) * 0.5f;
    glm::vec3 boxHalflength = box.max - boxCenter;
    glm::vec3 segmentMidpoint = (p0 + p1) * 0.5f;
    glm::vec3 segmentHalflength = p1 - segmentMidpoint;
    segmentMidpoint = segmentMidpoint - boxCenter;

    // Try world coordinate axes as separating axes
    float adx = std::abs(segmentHalflength.x);
    if (std::abs(segmentMidpoint.x) > boxHalflength.x + adx) {
        return false;
    }

    float ady = std::abs(segmentHalflength.y);
    if (std::abs(segmentMidpoint.y) > boxHalflength.y + ady) {
        return false;
    }

    float adz = std::abs(segmentHalflength.z);
    if (std::abs(segmentMidpoint.z) > boxHalflength.z + adz) {
        return false;
    }

    // Add in an epsilon term to counteract arithmetic errors when segment is
    // (near) parallel to a coordinate axis (see text for detail)
    adx += EPSILON;
    ady += EPSILON;
    adz += EPSILON;

    if (std::abs(segmentMidpoint.y * segmentHalflength.z - segmentMidpoint.z * segmentHalflength.y) >
        boxHalflength.y * adz + boxHalflength.z * ady) {
        return false;
    }

    if (std::abs(segmentMidpoint.z * segmentHalflength.x - segmentMidpoint.x * segmentHalflength.z) >
        boxHalflength.x * adz + boxHalflength.z * adx) {
        return false;
    }

    if (std::abs(segmentMidpoint.x * segmentHalflength.y - segmentMidpoint.y * segmentHalflength.x) >
        boxHalflength.x * ady + boxHalflength.y * adx) {
        return false;
    }

    return true;
}

} // namespace Engine
