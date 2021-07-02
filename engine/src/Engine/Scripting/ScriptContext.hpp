#pragma once

#include <lua.hpp>

#include "LuaBridge/LuaBridge.h"

namespace Engine {

class ScriptContext {
  protected:
    lua_State *m_State = nullptr;

  public:
    ScriptContext() {}
    virtual ~ScriptContext() {}

    lua_State *getState() { return m_State; }
};

} // namespace Engine