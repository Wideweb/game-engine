#pragma once

#include "CollisionDetection.hpp"
#include "System.hpp"

namespace Engine {

class CollisionSystem : public System {
  private:
    CollisionDetection m_CollisionDetection;

  public:
    virtual const void Update(ComponentManager &components) override;
};

} // namespace Engine
