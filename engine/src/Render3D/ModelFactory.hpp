#pragma once

#include "InstancedModel.hpp"
#include "Model.hpp"

#include <memory>

namespace Engine {

class ModelFactory {
  public:
    static std::shared_ptr<Model> createCube(float size = 1.0f);
    static std::shared_ptr<Model> createCube(float left, float right, float bottom, float top, float back, float front);
    static std::shared_ptr<InstancedModel> createPlane(float tileSize = 4.0f, int columns = 1, int rows = 1);
};

} // namespace Engine
