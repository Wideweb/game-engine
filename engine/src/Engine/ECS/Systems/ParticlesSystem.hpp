#pragma once

#include "BaseSystem.hpp"

namespace Engine {

class ParticlesSystem : public BaseSystem {
  public:
    using BaseSystem::BaseSystem;
    virtual void Attach(ComponentManager &components) const override;
    virtual void Update(ComponentManager &components) const override;

  private:
    void OnRemoveComponent(Entity entity) const;
};

} // namespace Engine
