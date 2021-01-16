#pragma once

#include "BaseSystem.hpp"

namespace Engine {

class Light3DSystem : public BaseSystem {
  public:
    using BaseSystem::BaseSystem;
    virtual void Update(ComponentManager &components) const override;
};

} // namespace Engine
