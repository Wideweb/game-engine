#pragma once

#include "Entity.hpp"
#include "Render.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

#include <string>
#include <vector>

namespace Engine {

class Render3DComponent {
  public:
    float scale;
    std::shared_ptr<Model> obj;

    Render3DComponent() {}
    Render3DComponent(std::shared_ptr<Model> &obj, float scale)
        : scale(scale), obj(obj) {}
};

} // namespace Engine