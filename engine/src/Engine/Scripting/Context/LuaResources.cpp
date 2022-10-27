#include "LuaResources.hpp"

#include "Application.hpp"

namespace Engine {

void LuaResources::add(lua_State *state) {
    luabridge::getGlobalNamespace(state)
        .beginNamespace("Resources")
        .beginNamespace("Materials")
        .addFunction("get", LuaResources::getMaterial)
        .endNamespace()
        .endNamespace();
}

Material* LuaResources::getMaterial(const std::string& name) {
    return Application::get().getMaterials().get(name);
}

} // namespace Engine