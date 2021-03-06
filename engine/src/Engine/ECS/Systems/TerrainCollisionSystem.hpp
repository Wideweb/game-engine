#pragma once

#include "BaseSystem.hpp"
#include "Collision3D.hpp"
#include "Entity.hpp"

namespace Engine {

class TerrainCollisionSystem : public BaseSystem {
  public:
    using BaseSystem::BaseSystem;
    virtual void Update(ComponentManager &components) const override;
};

} // namespace Engine
