#pragma once

#include "Entity.hpp"

#include <glm/vec3.hpp>
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
    std::vector<glm::vec3> vertices;
    std::unordered_set<Entity> entities;

    CollisionComponent() : CollisionComponent(0.0f, 0.0f, 0.0f) {}
    CollisionComponent(glm::vec3 box)
        : CollisionComponent(box.x, box.y, box.z) {}
    CollisionComponent(float width, float height, float depth) {
        float wHalf = width / 2;
        float hHalf = height / 2;
        float dHalf = depth / 2;

        vertices.emplace_back(-wHalf, hHalf, -dHalf);
        vertices.emplace_back(wHalf, hHalf, -dHalf);
        vertices.emplace_back(wHalf, hHalf, dHalf);
        vertices.emplace_back(-wHalf, hHalf, dHalf);

        vertices.emplace_back(-wHalf, -hHalf, -dHalf);
        vertices.emplace_back(wHalf, -hHalf, -dHalf);
        vertices.emplace_back(wHalf, -hHalf, dHalf);
        vertices.emplace_back(-wHalf, -hHalf, dHalf);
    }
};

} // namespace Engine