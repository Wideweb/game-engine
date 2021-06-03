#pragma once

#include "LuaEntity.hpp"
#include "Script.hpp"

#include "lua.hpp"
#include <LuaBridge/LuaBridge.h>

#include <memory>
#include <string>

namespace Engine {

class EntityScript : public ScriptBase {
  private:
    LuaEntity m_Entity;
    luabridge::LuaRef m_InitRef = nullptr;
    luabridge::LuaRef m_UpdateRef = nullptr;
    luabridge::LuaRef m_CollideRef = nullptr;

  public:
    EntityScript(std::string path, LuaEntity entity);

    void init();
    void update();
    void collide(const LuaEntity &entity);
};

} // namespace Engine