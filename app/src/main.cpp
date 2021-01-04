#include "Engine.hpp"

#include <glm/vec3.hpp>
#include <sstream>
#include <stdlib.h>
#include <string>

class AppLayer : public Engine::Layer {
  private:
  public:
    virtual void onAttach() override {
        auto box_model =
            Engine::ModelLoader::load("./assets/models/box/box.obj",
                                      "./assets/models/box/diffuse-map.png",
                                      "./assets/models/box/specular-map.png",
                                      "./assets/models/box/normal-map.png");

        auto cottage_model = Engine::ModelLoader::load(
            "./assets/models/cottage/cottage.obj",
            "./assets/models/cottage/cottage_diffuse.png",
            "./assets/models/cottage/cottage_specular.png",
            "./assets/models/cottage/cottage_normal.png");

        {
            auto player = m_Coordinator.CreateEntity("player");

            Engine::LocationComponent location(glm::vec3(0.0f, 0.03f, 0.0f),
                                               glm::vec3(0.0f, 0.0f, 0.0f));
            Engine::VelocityComponent velocity(0, glm::vec3(0.0f, 0.0f, 0.0f));
            Engine::CameraComponent camera;

            auto blackboard = std::make_shared<Engine::Blackboard>();
            blackboard->setValue<std::string>("layer",
                                              std::string("game_area"));
            blackboard->setValue<Engine::Entity>("entity", player);

            // clang-format off
            auto state = new Engine::BlackboardManager(
                blackboard,
                new Engine::ParallelTask({
                    new Engine::SelectorTask({
                        new Engine::SequenceTask({
                            new Engine::ControllerTask(Engine::KeyCode::A),
                            new Engine::RotateTask(glm::vec3(0.0f, glm::radians(-2.0f), 0.0f)),
                        }),
                        new Engine::SequenceTask({
                            new Engine::ControllerTask(Engine::KeyCode::D),
                            new Engine::RotateTask(glm::vec3(0.0f, glm::radians(2.0f), 0.0f)),
                        }),
                        new Engine::RotateTask(glm::vec3(0.0f, 0.0f, 0.0f))
                    }),

                    new Engine::SelectorTask({
                        new Engine::SequenceTask({
                            new Engine::ControllerTask(Engine::KeyCode::W),
                            new Engine::MoveTask(0.01),
                        }),
                        new Engine::SequenceTask({
                            new Engine::ControllerTask(Engine::KeyCode::S),
                            new Engine::MoveTask(-0.01f),
                        }),
                        new Engine::MoveTask(0)
                    })
                })
            );
            // clang-format on
            Engine::AIComponent ai{std::shared_ptr<Engine::Task>(state)};
            m_Coordinator.AddComponent(player, location);
            m_Coordinator.AddComponent(player, velocity);
            m_Coordinator.AddComponent(player, ai);
            m_Coordinator.AddComponent(player, camera);
        }

        {
            auto cottage = m_Coordinator.CreateEntity("cottage");
            Engine::LocationComponent location(glm::vec3(0.0f, 0.0f, 1.0f),
                                               glm::vec3(0.0f, 0.0f, 0.0f));
            Engine::Render3DComponent render{cottage_model, 0.01f};

            m_Coordinator.AddComponent(cottage, location);
            m_Coordinator.AddComponent(cottage, render);
        }

        for (size_t y = 0; y < 20; y++) {
            for (size_t x = 0; x < 20; x++) {
                {
                    std::ostringstream name;
                    name << "box" << x << y;
                    auto box = m_Coordinator.CreateEntity(name.str());

                    Engine::LocationComponent location(
                        glm::vec3(0.02f * x, 0.02f * y, -1.0f),
                        glm::vec3(0.0f, 0.0f, 0.0f));
                    Engine::Render3DComponent render{box_model, 0.01f};

                    m_Coordinator.AddComponent(box, location);
                    m_Coordinator.AddComponent(box, render);
                }
            }
        }

        {
            auto entity = m_Coordinator.CreateEntity("light");
            Engine::LocationComponent location(
                glm::vec3(0.0f, 2.0f, 0.0f),
                glm::vec3(0.0f, glm::radians(-90.0), 0.0f));
            Engine::VelocityComponent velocity(0.01,
                                               glm::vec3(-0.005f, 0.0, 0.0f));
            Engine::Light3DComponent light;
            Engine::Render3DComponent render{box_model, 0.01f};

            m_Coordinator.AddComponent(entity, location);
            m_Coordinator.AddComponent(entity, velocity);
            m_Coordinator.AddComponent(entity, light);
            m_Coordinator.AddComponent(entity, render);
        }
    }

    virtual void onUpdate() override {}

    virtual void onDetach() override {}
};

class MyApp : public Engine::Application {
  public:
    MyApp() { addLayer<AppLayer>("game_area"); }

    virtual ~MyApp() {}
};

Engine::Application *Engine::createApplication() { return new MyApp(); }
