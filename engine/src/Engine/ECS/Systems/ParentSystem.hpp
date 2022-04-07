#pragma once

#include "BaseSystem.hpp"

#include <array>
#include <unordered_map>
#include <unordered_set>

namespace Engine {

class ParentSystem : public BaseSystem {
  private:
    mutable std::unordered_map<Entity, std::unordered_set<Entity>> m_EntityChildren;
    mutable std::array<Entity, c_MaxEntities> m_ToRemove;

  public:
    using BaseSystem::BaseSystem;

    virtual void Attach(ComponentManager &components) const override;
    virtual void Update(ComponentManager &components) const override;
    void RemoveNode(ComponentManager &components, Entity entity) const;
};

} // namespace Engine
