#pragma once

#include "Entity.hpp"

namespace Engine {

class PhysicsComponent {
  public:
    float weight = 0.0f;

    PhysicsComponent() {}
    PhysicsComponent(float weight) : weight(weight) {}
};

} // namespace Engine