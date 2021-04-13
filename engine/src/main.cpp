#include "Engine.hpp"

#include "EntryScript.hpp"

#include <filesystem>
#include <glm/vec3.hpp>
#include <memory>
#include <string>

#include "lua.hpp"
#include <LuaBridge/LuaBridge.h>

class AppLayer : public Engine::Layer {
  private:
    std::filesystem::file_time_type m_ScriptLastWriteTime;

  public:
    using Layer::Layer;

    virtual void onAttach() override {
        Engine::EntryScript script("main.lua", Engine::LuaLayer(this));
        script.run();

        m_ScriptLastWriteTime = std::filesystem::last_write_time("main.lua");
    }

    virtual void onUpdate() override {
        auto writeTime = std::filesystem::last_write_time("main.lua");
        if (m_ScriptLastWriteTime != writeTime) {
            Engine::Application::get().reloadLayer<AppLayer>(m_Name);
        }
    }

    virtual void onDetach() override {}
};

class GameApp : public Engine::Application {
  public:
    GameApp() { addLayer<AppLayer>("game_area"); }

    virtual ~GameApp() {}
};

Engine::Application *Engine::createApplication() { return new GameApp(); }
