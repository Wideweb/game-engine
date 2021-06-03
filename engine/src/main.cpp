#include "Engine.hpp"

#include "EntryScript.hpp"

#include <filesystem>
#include <glm/vec3.hpp>
#include <memory>
#include <string>

#include "lua.hpp"
#include <LuaBridge/LuaBridge.h>

#include <iostream>

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
        auto ray = Engine::Application::get().getMousePicker().ray();
        auto pos = Engine::Application::get().getCamera().positionVec();
        auto result = m_Collision.Raycast(pos, ray, 100.0f);

        if (!result.empty()) {
            Engine::Entity entityId = result[0].id;
            if (m_Coordinator.HasComponent<Engine::Render3DComponent>(entityId)) {
                auto &render = m_Coordinator.GetComponent<Engine::Render3DComponent>(entityId);
                auto &collision = m_Coordinator.GetComponent<Engine::StaticCollisionComponent>(entityId);

                glm::mat4 revertModel = glm::inverse(glm::scale(glm::vec3(render.scale)));

                render.scale += 0.01f;
                render.updated = true;

                glm::mat4 model = glm::scale(glm::vec3(render.scale));
                for (auto &vertex : collision.vertices) {
                    vertex = glm::vec3(model * revertModel * glm::vec4(vertex, 1.0));
                }
                collision.updated = true;
            }
        }

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
