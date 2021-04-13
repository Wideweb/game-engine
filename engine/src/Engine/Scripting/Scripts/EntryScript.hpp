#pragma once

#include "LuaLayer.hpp"
#include "Script.hpp"

#include "lua.hpp"
#include <LuaBridge/LuaBridge.h>

#include <memory>
#include <string>

namespace Engine {

class EntryScript : public ScriptBase {
  private:
    LuaLayer m_Layer;
    luabridge::LuaRef m_EntryRef = nullptr;

  public:
    EntryScript(std::string path, LuaLayer layer);

    void run();
};

} // namespace Engine