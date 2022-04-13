#pragma once

#include "BaseSystem.hpp"
#include "Collision3D.hpp"
#include "Entity.hpp"

namespace Engine {

class StaticCollisionSystem : public BaseSystem {
  public:
    using BaseSystem::BaseSystem;
    virtual void Attach(ComponentManager &components) const override;
    virtual void Update(ComponentManager &components) const override;

  private:
    void OnRemoveComponent(Entity entity) const;
};

} // namespace Engine
