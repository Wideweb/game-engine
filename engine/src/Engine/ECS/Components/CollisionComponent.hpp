#pragma once

#include "Entity.hpp"
#include <glm/vec2.hpp>
#include <unordered_set>

namespace Engine {

struct BeginCollisionEvent {
    Entity first;
    Entity second;
};

struct EndCollisionEvent {
    Entity first;
    Entity second;
};

class CollisionComponent {
  public:
    std::vector<glm::vec2> vertices;
    std::unordered_set<Entity> entities;
    bool isStatic;

    CollisionComponent() : CollisionComponent(0, 0, false) {}
    CollisionComponent(float width, float height, bool isStatic)
        : isStatic(isStatic) {
        float wHalf = width / 2;
        float hHalf = height / 2;

        vertices.emplace_back(-wHalf, -hHalf);
        vertices.emplace_back(-wHalf, hHalf);
        vertices.emplace_back(wHalf, hHalf);
        vertices.emplace_back(wHalf, -hHalf);
    }

    CollisionComponent(const std::vector<glm::vec2> &vertices)
        : vertices(vertices) {}
};

} // namespace Engine