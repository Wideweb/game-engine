#pragma once

#include "Entity.hpp"

namespace Engine {

class ParentComponent {
  public:
    Entity entity = c_NoEntity, prevEntity = c_NoEntity;
    bool isDirty = true, destroyWithParent = true;

    void setEntity(Entity entity) {
        if (this->entity == entity) {
            return;
        }
        this->prevEntity = this->entity;
        this->entity = entity;
        this->isDirty = true;
    }

    ParentComponent() {}
    ParentComponent(Entity entity, bool destroyWithParent = true)
        : entity(entity), destroyWithParent(destroyWithParent) {}
};

} // namespace Engine