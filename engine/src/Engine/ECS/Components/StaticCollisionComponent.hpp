#pragma once

#include "Entity.hpp"

#include <glm/vec3.hpp>
#include <vector>

namespace Engine {

class StaticCollisionComponent {
  public:
    std::vector<glm::vec3> vertices;

    bool added = false;
    bool updated = false;

    StaticCollisionComponent() : StaticCollisionComponent(0.0f, 0.0f, 0.0f) {}
    StaticCollisionComponent(glm::vec3 box) : StaticCollisionComponent(box.x, box.y, box.z) {}
    StaticCollisionComponent(float width, float height, float depth) {
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