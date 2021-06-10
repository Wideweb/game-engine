#pragma once

namespace Engine {

class TerrainCollisionComponent {
  public:
    unsigned int width;
    unsigned int height;

    bool added = false;

    TerrainCollisionComponent() : width(0), height(0) {}
    TerrainCollisionComponent(unsigned int width, unsigned int height) : width(width), height(height) {}
};

} // namespace Engine