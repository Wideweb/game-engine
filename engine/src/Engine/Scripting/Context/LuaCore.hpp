#pragma once

#include "ScriptContext.hpp"

#include <lua.hpp>

#include "LuaBridge/LuaBridge.h"

#include <string>

namespace Engine {

class LuaCore : public ScriptContext {
  public:
    void static add(lua_State *state);
};

} // namespace Engine