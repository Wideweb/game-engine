#pragma once

#include <glm/vec3.hpp>
#include <vector>

namespace Engine {

struct CollisionShape {
    uint32_t id;
    std::vector<glm::vec3> vertices;
    bool isStatic;

    CollisionShape() {}
    CollisionShape(uint32_t id, std::vector<glm::vec3> vertices, bool isStatic)
        : id(id), vertices(std::move(vertices)), isStatic(isStatic) {}
};

} // namespace Engine