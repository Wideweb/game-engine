#pragma once

#include "BaseSystem.hpp"
#include "Entity.hpp"
#include "System.hpp"

#include <array>

namespace Engine {

class DestroySystem : public BaseSystem {
  private:
    mutable std::array<Entity, c_MaxEntities> m_ToRemove;

  public:
    using BaseSystem::BaseSystem;

    virtual void Update(ComponentManager &components) const override;
};

} // namespace Engine
