#pragma once

#include <memory>

#include "SpotLight.hpp"

namespace Engine {

class SpotLightComponent {
  public:
    SpotLight light;
    bool instanced = false;

    SpotLightComponent() {}
    SpotLightComponent(SpotLight light) : light(std::move(light)) {}
};

} // namespace Engine