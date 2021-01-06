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

            Engine::LocationComponent location(glm::vec3(10.0f, 10.0f, 0.0f),
                                               glm::vec3(0.0f, 0.0f, 0.0f));
            Engine::VelocityComponent velocity(0, glm::vec3(0.0f, 0.0f, 0.0f),
                                               glm::vec3(0.0f, 0.0f, 0.0f));
            Engine::CollisionComponent collision{1.0f, 5.0f, 1.0f, false};
            Engine::PhysicsComponent physics{0.01f};
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
                            new Engine::MoveTask(0.1f),
                        }),
                        new Engine::SequenceTask({
                            new Engine::ControllerTask(Engine::KeyCode::S),
                            new Engine::MoveTask(-0.1f),
                        }),
                        new Engine::MoveTask(0)
                    }),

                    new Engine::SequenceTask({
                            new Engine::OnGroundTask(),
                            new Engine::ControllerTask(Engine::KeyCode::Space),
                            new Engine::JumpTask(0.25f),
                        }),
                })
            );
            // clang-format on
            Engine::AIComponent ai{std::shared_ptr<Engine::Task>(state)};
            m_Coordinator.AddComponent(player, location);
            m_Coordinator.AddComponent(player, velocity);
            m_Coordinator.AddComponent(player, ai);
            m_Coordinator.AddComponent(player, camera);
            m_Coordinator.AddComponent(player, collision);
            m_Coordinator.AddComponent(player, physics);
        }

        {
            auto cottage = m_Coordinator.CreateEntity("cottage");
            Engine::LocationComponent location(glm::vec3(0.0f, 0.0f, 1.0f),
                                               glm::vec3(0.0f, 0.0f, 0.0f));
            Engine::Render3DComponent render{cottage_model, 0.01f};

            m_Coordinator.AddComponent(cottage, location);
            m_Coordinator.AddComponent(cottage, render);
        }

        {
            auto box = m_Coordinator.CreateEntity("box");

            Engine::LocationComponent location(glm::vec3(0.0f, 5.0f, -1.0f),
                                               glm::vec3(0.0f, 0.0f, 0.0f));
            Engine::VelocityComponent velocity(0, glm::vec3(0.0f, 0.0f, 0.0f),
                                               glm::vec3(0.0f, 0.0f, 0.0f));
            Engine::Render3DComponent render{box_model, 1.0f};
            Engine::CollisionComponent collision{2.0f, 2.0f, 2.0f, false};
            Engine::PhysicsComponent physics{0.001f};

            m_Coordinator.AddComponent(box, location);
            m_Coordinator.AddComponent(box, velocity);
            m_Coordinator.AddComponent(box, render);
            m_Coordinator.AddComponent(box, collision);
            m_Coordinator.AddComponent(box, physics);
        }

        {
            auto box = m_Coordinator.CreateEntity("box1");

            Engine::LocationComponent location(glm::vec3(4.0f, 10.0f, -1.0f),
                                               glm::vec3(0, 0.0f, 0.0f));
            Engine::VelocityComponent velocity(
                0.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
            Engine::Render3DComponent render{box_model, 1.0f};
            Engine::CollisionComponent collision{2.0f, 2.0f, 2.0f, false};
            Engine::PhysicsComponent physics{0.001f};

            m_Coordinator.AddComponent(box, location);
            m_Coordinator.AddComponent(box, velocity);
            m_Coordinator.AddComponent(box, render);
            m_Coordinator.AddComponent(box, collision);
            m_Coordinator.AddComponent(box, physics);
        }

        {
            auto box = m_Coordinator.CreateEntity("floor");

            Engine::LocationComponent location(glm::vec3(0.0f, -20.0f, 0.0f),
                                               glm::vec3(0, 0.0f, 0.0f));
            Engine::VelocityComponent velocity(
                0.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
            Engine::Render3DComponent render{box_model, 20.0f};
            Engine::CollisionComponent collision{40.0f, 40.0f, 40.0f, true};

            m_Coordinator.AddComponent(box, location);
            m_Coordinator.AddComponent(box, velocity);
            m_Coordinator.AddComponent(box, render);
            m_Coordinator.AddComponent(box, collision);
        }

        // for (size_t y = 0; y < 20; y++) {
        //     for (size_t x = 0; x < 20; x++) {
        //         {
        //             std::ostringstream name;
        //             name << "box" << x << y;
        //             auto box = m_Coordinator.CreateEntity(name.str());

        //             Engine::LocationComponent location(
        //                 glm::vec3(0.02f * x, 0.02f * y, -1.0f),
        //                 glm::vec3(0.0f, 0.0f, 0.0f));
        //             Engine::Render3DComponent render{box_model, 0.01f};
        //             Engine::VelocityComponent velocity(
        //                 0.002f, glm::vec3(0.0f, 0.0f, 0.0f));

        //             m_Coordinator.AddComponent(box, location);
        //             m_Coordinator.AddComponent(box, render);
        //             m_Coordinator.AddComponent(box, velocity);
        //         }
        //     }
        // }

        // {
        //     auto entity = m_Coordinator.CreateEntity("light");
        //     Engine::LocationComponent location(
        //         glm::vec3(0.0f, 10.0f, 0.0f),
        //         glm::vec3(0.0f, glm::radians(-90.0), 0.0f));
        //     Engine::VelocityComponent velocity(0.1f,
        //                                        glm::vec3(-0.01f, 0.0f, 0.0f),
        //                                        glm::vec3(0.0f, 0.0f, 0.0f));
        //     Engine::Light3DComponent light;
        //     Engine::Render3DComponent render{box_model, 1.0f};

        //     m_Coordinator.AddComponent(entity, location);
        //     m_Coordinator.AddComponent(entity, velocity);
        //     m_Coordinator.AddComponent(entity, light);
        //     m_Coordinator.AddComponent(entity, render);
        // }

        {
            auto entity = m_Coordinator.CreateEntity("light2");
            Engine::LocationComponent location(glm::vec3(50.0f, 50.0f, 0.0f),
                                               glm::vec3(0.0f, 0.0f, 0.0f));
            Engine::Light3DComponent light;
            light.light.ambient = glm::vec3(1.0f);
            light.light.quadratic = 0.0f;
            Engine::Render3DComponent render{box_model, 1.0f};

            m_Coordinator.AddComponent(entity, location);
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
