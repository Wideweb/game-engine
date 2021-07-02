#pragma once

#include "Entity.hpp"
#include "Input.hpp"
#include "Layer.hpp"
#include "ScriptContext.hpp"

#include "lua.hpp"

#include "LuaBridge/LuaBridge.h"

#include <memory>
#include <string>

namespace Engine {

class LuaInput {
  public:
    template <KeyCode code> int static getKeyCode() { return static_cast<int>(code); }

    bool static isKeyPressed(int key);

    void static add(lua_State *state);
};

} // namespace Engine