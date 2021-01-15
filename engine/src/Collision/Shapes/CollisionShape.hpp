#pragma once

#include <glm/vec3.hpp>
#include <vector>

#include "AABB.hpp"

namespace Engine {

template <typename T> struct CollisionShape {
    T id;
    std::vector<glm::vec3> vertices;
    AABB box;

    CollisionShape() {}
    CollisionShape(T id, const std::vector<glm::vec3> &vertices)
        : id(id), vertices(vertices), box(AABB(vertices)) {}
};

} // namespace Engine