#pragma once

#include "System.hpp"

namespace Engine {

class PhysicsSystem : public System {
  public:
    virtual const void Update(ComponentManager &components) override;
};

} // namespace Engine
