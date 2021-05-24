#pragma once

#include <memory>

#include "SpotLight.hpp"

namespace Engine {

class SpotLightComponent {
  public:
    SpotLight light;

    SpotLightComponent() {}
    SpotLightComponent(SpotLight light) : light(std::move(light)) {}
};

} // namespace Engine