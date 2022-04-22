#include "EntityScript.hpp"
#include "LuaCore.hpp"
#include "LuaInput.hpp"
#include "LuaRender.hpp"

#include <iostream>
#include <stdexcept>

namespace Engine {

EntityScript::EntityScript(std::string path, LuaEntity entity) : m_Entity(entity) {
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    LuaCore::add(L);
    LuaInput::add(L);
    LuaRender::add(L);
    LuaEntity::add(L);

    int error = luaL_loadfile(L, path.c_str());
    if (error && lua_gettop(L)) {
        std::cerr << "stack = " << lua_gettop(L) << "\n";
        std::cerr << "error = " << error << "\n";
        std::cerr << "message = " << lua_tostring(L, -1) << "\n";
        lua_pop(L, 1);
        error = lua_pcall(L, 0, 0, 0);
        throw std::runtime_error("Unable to find lua file");
    }

    error = lua_pcall(L, 0, 0, 0);
    while (error && lua_gettop(L)) {
        std::cerr << "stack = " << lua_gettop(L) << "\n";
        std::cerr << "error = " << error << "\n";
        std::cerr << "message = " << lua_tostring(L, -1) << "\n";
        lua_pop(L, 1);
        error = lua_pcall(L, 0, 0, 0);
    }

    luabridge::LuaRef initRef = luabridge::getGlobal(L, "init");
    if (initRef.isNil()) {
        std::cerr << "init function not found in lua" << std::endl;
        return;
    }
    if (!initRef.isFunction()) {
        std::cerr << "Wrong init type" << std::endl;
        return;
    }

    m_InitRef = initRef;

    luabridge::LuaRef updateRef = luabridge::getGlobal(L, "update");
    if (updateRef.isNil()) {
        std::cerr << "update function not found in lua" << std::endl;
        return;
    }
    if (!updateRef.isFunction()) {
        std::cerr << "Wrong update type" << std::endl;
        return;
    }

    m_UpdateRef = updateRef;
    m_CollideRef = luabridge::getGlobal(L, "collide");
    name = path;
}

void EntityScript::init() {
    try {
        m_InitRef(m_Entity);
    } catch (const std::string &message) {
        throw std::invalid_argument("EntityScript::init: " + message);
    }
}

void EntityScript::update() {
    try {
        m_UpdateRef(m_Entity);
    } catch (const std::string &message) {
        throw std::invalid_argument("EntityScript::update: " + message);
    }
}

void EntityScript::collide(const LuaEntity &entity) {
    try {
        m_CollideRef(m_Entity, entity);
    } catch (const std::string &message) {
        throw std::invalid_argument("EntityScript::collide: " + message);
    }
}

} // namespace Engine