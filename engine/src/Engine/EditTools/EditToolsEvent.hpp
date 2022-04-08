#pragma once

#include "Entity.hpp"
#include "ObjectPool.hpp"

namespace Engine {

class EditToolsEvent {
  public:
    enum class Type { None = 0, SelectEntity = 1, DeleteEntity = 2 };

    Type type;
    Entity entity;
};

using EditToolsEventPool = ObjectPool<EditToolsEvent, 100>;

} // namespace Engine