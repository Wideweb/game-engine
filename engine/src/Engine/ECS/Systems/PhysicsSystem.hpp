#pragma once

#include "System.hpp"

namespace Engine {

class PhysicsSystem : public System {
  public:
    virtual void Update(ComponentManager &components) const override;
};

} // namespace Engine
