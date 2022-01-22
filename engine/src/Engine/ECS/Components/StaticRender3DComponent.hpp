#pragma once

#include <string>
#include <vector>

namespace Engine {

class StaticRender3DComponent {
  public:
    float scale;
    std::string model;
    bool instanced = false;
    PolygonMode mode = PolygonMode::Fill;

    StaticRender3DComponent() {}
    StaticRender3DComponent(std::string model, float scale) : scale(scale), model(std::move(model)) {}
};

} // namespace Engine