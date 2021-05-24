#pragma once

#include <memory>

#include "DirectedLight.hpp"

namespace Engine {

class DirectedLightComponent {
  public:
    DirectedLight light;

    DirectedLightComponent() {}
    DirectedLightComponent(DirectedLight light) : light(std::move(light)) {}
};

} // namespace Engine