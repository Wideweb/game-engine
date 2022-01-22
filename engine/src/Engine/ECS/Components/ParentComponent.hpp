#pragma once

#include "Entity.hpp"

namespace Engine {

class ParentComponent {
  public:
    Entity entity = c_NoEntity;

    ParentComponent() {}
    ParentComponent(Entity entity) : entity(entity) {}
};

} // namespace Engine