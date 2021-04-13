#pragma once

#include "BaseSystem.hpp"

namespace Engine {

class BehaviourSystem : public BaseSystem {
  public:
    using BaseSystem::BaseSystem;
    virtual void Update(ComponentManager &components) const override;
};

} // namespace Engine
