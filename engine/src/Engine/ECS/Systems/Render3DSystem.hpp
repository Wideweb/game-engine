#pragma once

#include "BaseSystem.hpp"

namespace Engine {

class Render3DSystem : public BaseSystem {
  public:
    using BaseSystem::BaseSystem;
    virtual void Update(ComponentManager &components) const override;
};

} // namespace Engine
