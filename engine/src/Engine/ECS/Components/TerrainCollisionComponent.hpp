#pragma once

namespace Engine {

class TerrainCollisionComponent {
  public:
    unsigned int columns = 1, rows = 1;

    bool added, updated = false;
};

} // namespace Engine