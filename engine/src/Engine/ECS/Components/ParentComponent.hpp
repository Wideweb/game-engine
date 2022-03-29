#pragma once

#include "Entity.hpp"

namespace Engine {

class ParentComponent {
  public:
    Entity entity = c_NoEntity, prevEntity = c_NoEntity;
    bool isDirty = true;

    void setEntity(Entity entity) {
        this->prevEntity = entity;
        this->entity = entity;
        this->isDirty = true;
    }

    ParentComponent() {}
    ParentComponent(Entity entity) : entity(entity) {}
};

} // namespace Engine