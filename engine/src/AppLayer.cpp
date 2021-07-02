#include "AppLayer.hpp"

#include "EntryScript.hpp"

#include <string>

#include "LuaBridge/LuaBridge.h"
#include "lua.hpp"

void AppLayer::onAttach() {
    Engine::EntryScript script("main.lua", Engine::LuaLayer(this));
    script.run();
    m_ScriptLastWriteTime = std::filesystem::last_write_time("main.lua");
}

void AppLayer::onUpdate() {
    auto writeTime = std::filesystem::last_write_time("main.lua");
    if (m_ScriptLastWriteTime != writeTime) {
        Engine::Application::get().reloadLayer<AppLayer>(m_Name);
    }
}

void AppLayer::onDetach() {}

void AppLayer::onMouseEvent(Engine::MouseEvent &event) {}
