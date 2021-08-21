#pragma once

#include "ScriptContext.hpp"

#include <lua.hpp>
#include <memory>
#include <type_traits>

#include "LuaBridge/LuaBridge.h"

namespace Engine {

class ScriptBase {
  public:
    virtual ~ScriptBase() {}
};

template <typename TContext, typename = std::enable_if_t<std::is_base_of_v<ScriptContext, TContext>>>
class Script : public ScriptBase {
  private:
    std::shared_ptr<TContext> m_Context;
    luabridge::LuaRef m_EntryRef = nullptr;

  public:
    Script(std::string path, std::string entry, std::shared_ptr<TContext> context) : m_Context(context) {
        lua_State *L = m_Context->getState();

        int error = luaL_loadfile(L, path.c_str());
        while (error && lua_gettop(L)) {
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

        luabridge::LuaRef entryRef = luabridge::getGlobal(L, entry.c_str());
        if (entryRef.isNil()) {
            std::cerr << "entry function not found in lua" << std::endl;
            return;
        }
        if (!entryRef.isFunction()) {
            std::cerr << "Wrong main type" << std::endl;
            return;
        }

        m_EntryRef = entryRef;
    }

    void run() { m_EntryRef(*m_Context); }
};

} // namespace Engine