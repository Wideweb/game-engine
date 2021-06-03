#pragma once

#include "BaseSystem.hpp"
#include "LuaEntity.hpp"

namespace Engine {

class BehaviourSystem : public BaseSystem {
  private:
    mutable LuaEntity m_LuaEntity;

  public:
    using BaseSystem::BaseSystem;
    virtual void Update(ComponentManager &components) const override;
};

} // namespace Engine
