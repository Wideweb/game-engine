#pragma once

#include "BaseSystem.hpp"

#include "SpotLightComponent.hpp"

namespace Engine {

class SpotLightSystem : public BaseSystem {
  public:
    using BaseSystem::BaseSystem;
    virtual void Attach(ComponentManager &components) const override;
    virtual void Update(ComponentManager &components) const override;

  private:
    void OnRemoveComponent(Entity entity) const;
    void OnModelChange(Entity entity, SpotLightComponent &render) const;
};

} // namespace Engine
