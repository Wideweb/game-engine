#pragma once

#include "Collision3D.hpp"
#include "Entity.hpp"
#include "System.hpp"

namespace Engine {

class CollisionSystem : public System {
  public:
    virtual void Update(ComponentManager &components) override;
};

} // namespace Engine
