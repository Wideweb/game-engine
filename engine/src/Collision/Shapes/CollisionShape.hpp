#pragma once

#include <glm/vec2.hpp>
#include <vector>

namespace Engine {

struct CollisionShape {
    uint32_t id;
    std::vector<glm::vec2> vertices;
    bool isStatic;

    CollisionShape() {}
    CollisionShape(uint32_t id, const std::vector<glm::vec2> &vertices,
                   bool isStatic)
        : id(id), vertices(vertices), isStatic(isStatic) {}
};

} // namespace Engine