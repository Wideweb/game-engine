#pragma once

#include "BaseSystem.hpp"

namespace Engine {

class ParticlesSystem : public BaseSystem {
  public:
    using BaseSystem::BaseSystem;
    virtual void Update(ComponentManager &components) const override;
};

} // namespace Engine
