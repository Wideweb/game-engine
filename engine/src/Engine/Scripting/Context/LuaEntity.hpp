#pragma once

#include "Entity.hpp"
#include "Layer.hpp"
#include "ScriptContext.hpp"

#include "lua.hpp"
#include <LuaBridge/LuaBridge.h>

namespace Engine {

class LuaEntity {
  private:
    Entity m_Entity;
    Layer *m_Layer;

  public:
    LuaEntity(Entity entity, Layer *m_Layer);

    template <typename T> void addComponent(T component) {
        m_Layer->getCoordinator().AddComponent(m_Entity, component);
    }

    template <typename T> T &getComponent() {
        return m_Layer->getCoordinator().GetComponent<T>(m_Entity);
    }

    void static add(lua_State *state);
};

} // namespace Engine