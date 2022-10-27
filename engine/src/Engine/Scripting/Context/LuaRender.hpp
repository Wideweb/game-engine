#pragma once

#include <glm/vec2.hpp>
#include <lua.hpp>

namespace Engine {

class LuaRender {
  public:
    static glm::vec2 viewport();

    void static add(lua_State *state);
};

} // namespace Engine