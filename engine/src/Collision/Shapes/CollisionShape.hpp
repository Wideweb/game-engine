#pragma once

#include <glm/vec3.hpp>
#include <vector>

#include "AABB.hpp"

namespace Engine {

enum class CollisionShapeType { Object = 0, Terrain = 1 };

template <typename T> struct CollisionShape {
    T id;
    std::vector<glm::vec3> vertices;
    AABB box;
    CollisionShapeType type;

    unsigned int width;
    unsigned int height;

    CollisionShape() {}
    CollisionShape(T id, const std::vector<glm::vec3> &vertices,
                   CollisionShapeType type)
        : id(id), vertices(vertices), box(AABB(vertices)), type(type) {}
};

} // namespace Engine