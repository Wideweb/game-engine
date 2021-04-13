#include "LuaInput.hpp"

#include "Application.hpp"

namespace Engine {

void LuaInput::add(lua_State *state) {
    luabridge::getGlobalNamespace(state)
        .beginNamespace("Input")
        .addFunction("isKeyPressed", LuaInput::isKeyPressed)
        .beginNamespace("Key")
        .addProperty("A", LuaInput::getKeyCode<KeyCode::A>)
        .addProperty("D", LuaInput::getKeyCode<KeyCode::D>)
        .addProperty("S", LuaInput::getKeyCode<KeyCode::S>)
        .addProperty("W", LuaInput::getKeyCode<KeyCode::W>)
        .addProperty("Space", LuaInput::getKeyCode<KeyCode::Space>)
        .endNamespace()
        .endNamespace();
}

bool LuaInput::isKeyPressed(int key) {
    KeyCode code = static_cast<KeyCode>(key);
    return Application::get().getInput().IsKeyPressed(code);
}

} // namespace Engine