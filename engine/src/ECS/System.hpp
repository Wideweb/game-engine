#pragma once

#include "ComponentManager.hpp"
#include "EntityManager.hpp"

#include <set>

namespace Engine {

class System {
  public:
    std::set<Entity> m_Entities;

    virtual ~System() = default;
    virtual const void Update(ComponentManager &components) = 0;
};

} // namespace Engine
