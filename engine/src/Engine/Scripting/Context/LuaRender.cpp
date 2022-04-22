#include "LuaRender.hpp"

#include "Application.hpp"

#include "LuaBridge/LuaBridge.h"

namespace Engine {

void LuaRender::add(lua_State *state) {
    luabridge::getGlobalNamespace(state)
        .beginNamespace("Render")
        .addFunction("viewport", &LuaRender::viewport)
        .endNamespace();
}

glm::vec2 LuaRender::viewport() {
    auto &viewport = Application::get().getRender().getViewport();
    return glm::vec2(viewport.width, viewport.height);
}

} // namespace Engine