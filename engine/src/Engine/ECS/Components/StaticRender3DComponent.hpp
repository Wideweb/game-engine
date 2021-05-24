#pragma once

#include "ModelInstanceManager.hpp"

#include <string>
#include <vector>

namespace Engine {

class StaticRender3DComponent {
  public:
    float scale;
    std::string model;
    ModelInstance instance = NoModelInstance;

    StaticRender3DComponent() {}
    StaticRender3DComponent(std::string model, float scale) : scale(scale), model(std::move(model)) {}
};

} // namespace Engine