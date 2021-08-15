#include "Engine.hpp"

#include "AppLayer.hpp"
#include "EditToolsLayer.hpp"

class GameApp : public Engine::Application {
  public:
    GameApp(Engine::ApplicationSettings settings) : Engine::Application(settings) {
        addLayer<AppLayer>("game_area").renderSettings.hdr = settings.hdr;

        if (settings.edit) {
            addLayer<Engine::EditToolsLayer>("edit_tools").renderSettings.hdr = false;
        }
    }

    virtual ~GameApp() {}
};

Engine::Application *Engine::createApplication(Engine::ApplicationSettings settings) { return new GameApp(settings); }
