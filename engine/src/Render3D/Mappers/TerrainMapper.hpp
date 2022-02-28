#pragma once

#include "Model.hpp"

#include <memory>
#include <vector>

namespace Engine {

class TerrainMapper {
  public:
    std::shared_ptr<Model> map(const std::vector<float> &heightMap, unsigned int terrainWidth,
                               unsigned int terrainHeight);
};

} // namespace Engine