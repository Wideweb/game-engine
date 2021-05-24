#pragma once

#include "BaseSystem.hpp"

namespace Engine {

class DirectedLightSystem : public BaseSystem {
  public:
    using BaseSystem::BaseSystem;
    virtual void Update(ComponentManager &components) const override;
};

} // namespace Engine
