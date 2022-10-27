#pragma once

#include "Material.hpp"

#include "lua.hpp"

#include "LuaBridge/LuaBridge.h"

#include <string>

namespace Engine {

class LuaResources {
  public:
    static void add(lua_State *state);

    static Material* getMaterial(const std::string& name);
};

} // namespace Engine