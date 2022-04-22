#include "EntryScript.hpp"
#include "LuaCore.hpp"
#include "LuaEntity.hpp"
#include "LuaInput.hpp"
#include "LuaRender.hpp"

#include <stdexcept>

namespace Engine {

EntryScript::EntryScript(std::string path, LuaLayer layer) : m_Layer(layer) {
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    LuaCore::add(L);
    LuaInput::add(L);
    LuaRender::add(L);
    LuaEntity::add(L);
    LuaLayer::add(L);

    if (luaL_loadfile(L, path.c_str())) {
        throw std::runtime_error("Unable to find lua file");
    }

    int error = lua_pcall(L, 0, 0, 0);
    while (error && lua_gettop(L)) {
        std::cerr << "stack = " << lua_gettop(L) << "\n";
        std::cerr << "error = " << error << "\n";
        std::cerr << "message = " << lua_tostring(L, -1) << "\n";
        lua_pop(L, 1);
        error = lua_pcall(L, 0, 0, 0);
    }

    luabridge::LuaRef entryRef = luabridge::getGlobal(L, "main");
    if (entryRef.isNil()) {
        std::cerr << "entry function not found in lua" << std::endl;
        return;
    }
    if (!entryRef.isFunction()) {
        std::cerr << "Wrong main type" << std::endl;
        return;
    }

    m_EntryRef = entryRef;
    name = path;
}

void EntryScript::run() { m_EntryRef(m_Layer); }

} // namespace Engine