#pragma once

#include "Entity.hpp"
#include "ModelInstanceManager.hpp"
#include "Render.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

#include <string>
#include <vector>

namespace Engine {

class Render3DComponent {
  public:
    float scale;
    std::string model;
    ModelInstance instance = NoModelInstance;

    Render3DComponent() {}
    Render3DComponent(std::string model, float scale)
        : scale(scale), model(std::move(model)) {}
};

} // namespace Engine