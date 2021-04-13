#pragma once

#include <memory>

#include "Light.hpp"

namespace Engine {

class Light3DComponent {
  public:
    Light light;

    Light3DComponent() {}
    Light3DComponent(Light light) : light(std::move(light)) {}
};

} // namespace Engine