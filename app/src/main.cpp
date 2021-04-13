#include "Engine.hpp"

#include <glm/vec3.hpp>
#include <sstream>
#include <stdlib.h>
#include <string>

class AppLayer : public Engine::Layer {
  public:
    using Layer::Layer;

    virtual void onAttach() override {
        auto skybox_model = Engine::ModelLoader::loadSkybox(
            {"./assets/models/skybox/right.jpg",
             "./assets/models/skybox/left.jpg",
             "./assets/models/skybox/top.jpg",
             "./assets/models/skybox/bottom.jpg",
             "./assets/models/skybox/front.jpg",
             "./assets/models/skybox/back.jpg"});
        getScene().setSkybox(skybox_model);

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

        Engine::Application::get().getModels().RegisterModel("box", box_model);
        Engine::Application::get().getModels().RegisterModel("cottage",
                                                             cottage_model);

        {
            auto player = m_Coordinator.CreateEntity("player");

            Engine::LocationComponent location(glm::vec3(10.0f, 10.0f, 0.0f),
                                               glm::vec3(0.0f, 0.0f, 0.0f));
            Engine::VelocityComponent velocity;
            Engine::Render3DComponent render{"box", 0.5f};
            Engine::CollisionComponent collision{1.0f, 1.0f, 1.0f};
            Engine::PhysicsComponent physics{0.01f};
            Engine::CameraComponent camera;
            camera.offset = glm::vec3(-5.0f, 1.0f, 0.0f);

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
            m_Coordinator.AddComponent(player, render);
            m_Coordinator.AddComponent(player, ai);
            m_Coordinator.AddComponent(player, camera);
            m_Coordinator.AddComponent(player, collision);
            m_Coordinator.AddComponent(player, physics);
        }

        // {
        //     auto cottage = m_Coordinator.CreateEntity("cottage");
        //     Engine::LocationComponent location(glm::vec3(-10.0f, 0.0f, 1.0f),
        //                                        glm::vec3(0.0f, 0.0f, 0.0f));
        //     Engine::StaticRender3DComponent render{"cottage", 0.5f};
        //     Engine::StaticCollisionComponent collision{14.0f, 18.0f, 8.0f};

        //     m_Coordinator.AddComponent(cottage, location);
        //     m_Coordinator.AddComponent(cottage, render);
        //     m_Coordinator.AddComponent(cottage, collision);
        // }

        // {
        //     auto box = m_Coordinator.CreateEntity("floor");

        //     Engine::LocationComponent location(glm::vec3(0.0f, -20.0f, 0.0f),
        //                                        glm::vec3(0.0f, 0.0f, 0.0f));
        //     Engine::StaticRender3DComponent render{"box", 20.0f};
        //     Engine::StaticCollisionComponent collision{40.0f, 40.0f, 40.0f};

        //     m_Coordinator.AddComponent(box, location);
        //     m_Coordinator.AddComponent(box, render);
        //     m_Coordinator.AddComponent(box, collision);
        // }

        // for (size_t z = 0; z < 10; z++) {
        //     for (size_t y = 0; y < 10; y++) {
        //         for (size_t x = 0; x < 10; x++) {
        //             {
        //                 std::ostringstream name;
        //                 name << "box" << x << y;
        //                 auto box = m_Coordinator.CreateEntity(name.str());

        //                 Engine::LocationComponent location(
        //                     glm::vec3(-20.0f - 2.0f * z, 2.0f * y,
        //                               2.0f * x - 5),
        //                     glm::vec3(0.0f, 0.0f, 0.0f));
        //                 Engine::StaticRender3DComponent render{"box", 0.25f};
        //                 Engine::StaticCollisionComponent collision{0.5f,
        //                 0.5f,
        //                                                            0.5f};

        //                 m_Coordinator.AddComponent(box, location);
        //                 m_Coordinator.AddComponent(box, render);
        //                 m_Coordinator.AddComponent(box, collision);
        //             }
        //         }
        //     }
        // }

        // {
        //     auto entity = m_Coordinator.CreateEntity("light");
        //     Engine::LocationComponent location(
        //         glm::vec3(0.0f, 10.0f, 0.0f),
        //         glm::vec3(0.0f, glm::radians(-90.0), 0.0f));
        //     Engine::VelocityComponent velocity(0.1f,
        //                                        glm::vec3(-0.01f, 0.0f,
        //                                        0.0f), glm::vec3(0.0f,
        //                                        0.0f, 0.0f));
        //     Engine::Light3DComponent light;
        //     Engine::Render3DComponent render{box_model, 1.0f};

        //     m_Coordinator.AddComponent(entity, location);
        //     m_Coordinator.AddComponent(entity, velocity);
        //     m_Coordinator.AddComponent(entity, light);
        //     m_Coordinator.AddComponent(entity, render);
        // }

        // {
        //     auto entity = m_Coordinator.CreateEntity("light3");
        //     Engine::LocationComponent location(glm::vec3(10.0f, 5.0f, 10.0f),
        //                                        glm::vec3(0.0f, 0.0f, 0.0f));
        //     Engine::Light3DComponent light;
        //     light.light.quadratic = 0.0f;
        //     light.light.linear = 0.0f;
        //     Engine::Render3DComponent render{"box", 0.05f};

        //     m_Coordinator.AddComponent(entity, location);
        //     m_Coordinator.AddComponent(entity, light);
        //     m_Coordinator.AddComponent(entity, render);
        // }

        // {
        //     auto entity = m_Coordinator.CreateEntity("light2");
        //     Engine::LocationComponent
        //     location(glm::vec3(-10.0f, 5.0f, 10.0f),
        //                                        glm::vec3(0.0f, 0.0f, 0.0f));
        //     Engine::Light3DComponent light;
        //     light.light.quadratic = 0.0f;
        //     light.light.linear = 0.0f;
        //     Engine::Render3DComponent render{"box", 0.05f};

        //     m_Coordinator.AddComponent(entity, location);
        //     m_Coordinator.AddComponent(entity, light);
        //     m_Coordinator.AddComponent(entity, render);
        // }

        loadEntity("./assets/floor.lua");
        loadEntity("./assets/cottage.lua");
        loadEntity("./assets/box.lua");
        loadEntity("./assets/box1.lua");
        loadEntity("./assets/light.lua");
    }

    virtual void onUpdate() override {}

    virtual void onDetach() override {}

    void loadEntity(std::string path) {
        lua_State *L = luaL_newstate();
        luaL_openlibs(L);

        if (luaL_loadfile(L, path.c_str())) {
            throw std::runtime_error("Unable to find lua file");
        }

        int error = lua_pcall(L, 0, 0, 0);
        while (error && lua_gettop(L)) {
            std::cerr << "stack = " << lua_gettop(L) << "\n";
            std::cerr << "error = " << error << "\n";
            std::cerr << "message = " << lua_tostring(L, -1) << "\n";
            lua_pop(L, 1);
            error = lua_pcall(L, 0, 0, 0);
        }

        luabridge::LuaRef entityRef = luabridge::getGlobal(L, "entity");
        if (entityRef.isNil()) {
            std::cerr << "Entity not found!" << std::endl;
            return;
        }
        if (!entityRef.isTable()) {
            std::cerr << "Wrong entity type!" << std::endl;
            return;
        }

        luabridge::LuaRef nameRef = entityRef["name"];
        if (nameRef.isNil()) {
            std::cerr << "Entity name not found!" << std::endl;
            return;
        }
        if (!nameRef.isString()) {
            std::cerr << "Wrong entity name type!" << std::endl;
            return;
        }

        std::string name = nameRef.cast<std::string>();
        auto entity = m_Coordinator.CreateEntity(name);

        luabridge::LuaRef staticRef = entityRef["static"];
        bool isStatic = false;

        if (!staticRef.isNil() && staticRef.isBool()) {
            isStatic = staticRef.cast<bool>();
        }

        for (auto &&pair : luabridge::pairs(entityRef)) {
            if (pair.first == "name") {
                continue;
            }

            if (pair.first == "position") {
                luabridge::LuaRef &positionRef = pair.second;

                if (positionRef.isNil() || !positionRef.isTable()) {
                    std::cerr << "Wrong position" << std::endl;
                    continue;
                }

                glm::vec3 position = glm::vec3(positionRef[1].cast<float>(),
                                               positionRef[2].cast<float>(),
                                               positionRef[3].cast<float>());

                Engine::LocationComponent location(position,
                                                   glm::vec3(0.0f, 0.0f, 0.0f));
                m_Coordinator.AddComponent(entity, location);
            }

            if (pair.first == "render") {
                luabridge::LuaRef &renderRef = pair.second;
                luabridge::LuaRef modelRef = renderRef["model"];
                luabridge::LuaRef scaleRef = renderRef["scale"];

                std::string model;
                float scale = 1.0f;

                if (modelRef.isNil() || !modelRef.isString()) {
                    std::cerr << "Wrong render model" << std::endl;
                    continue;
                }
                model = modelRef.cast<std::string>();

                if (!scaleRef.isNil() && scaleRef.isNumber()) {
                    scale = scaleRef.cast<float>();
                }

                if (isStatic) {
                    Engine::StaticRender3DComponent render{model, scale};
                    m_Coordinator.AddComponent(entity, render);
                } else {
                    Engine::Render3DComponent render{model, scale};
                    m_Coordinator.AddComponent(entity, render);
                }
            }

            if (pair.first == "collision") {
                luabridge::LuaRef collisionRef = pair.second;

                glm::vec3 collisionBox =
                    glm::vec3(collisionRef[1].cast<float>(),
                              collisionRef[2].cast<float>(),
                              collisionRef[3].cast<float>());

                if (isStatic) {
                    Engine::StaticCollisionComponent collision(collisionBox);
                    m_Coordinator.AddComponent(entity, collision);
                } else {
                    Engine::CollisionComponent collision(collisionBox);
                    m_Coordinator.AddComponent(entity, collision);
                }
            }

            if (pair.first == "light") {
                luabridge::LuaRef lightRef = pair.second;

                luabridge::LuaRef ambientRef = lightRef["ambient"];
                luabridge::LuaRef diffuseRef = lightRef["diffuse"];
                luabridge::LuaRef specularRef = lightRef["specular"];

                luabridge::LuaRef constantRef = lightRef["constant"];
                luabridge::LuaRef linearRef = lightRef["linear"];
                luabridge::LuaRef quadraticRef = lightRef["quadratic"];

                Engine::Light light;

                if (!ambientRef.isNil() && ambientRef.isTable()) {
                    light.ambient = glm::vec3(ambientRef[1].cast<float>(),
                                              ambientRef[2].cast<float>(),
                                              ambientRef[3].cast<float>());
                }

                if (!diffuseRef.isNil() && diffuseRef.isTable()) {
                    light.diffuse = glm::vec3(diffuseRef[1].cast<float>(),
                                              diffuseRef[2].cast<float>(),
                                              diffuseRef[3].cast<float>());
                }

                if (!specularRef.isNil() && specularRef.isTable()) {
                    light.specular = glm::vec3(specularRef[1].cast<float>(),
                                               specularRef[2].cast<float>(),
                                               specularRef[3].cast<float>());
                }

                if (!constantRef.isNil() && constantRef.isNumber()) {
                    light.constant = constantRef.cast<float>();
                }

                if (!linearRef.isNil() && linearRef.isNumber()) {
                    light.linear = linearRef.cast<float>();
                }

                if (!quadraticRef.isNil() && quadraticRef.isNumber()) {
                    light.quadratic = quadraticRef.cast<float>();
                }

                Engine::Light3DComponent light3D(light);
                m_Coordinator.AddComponent(entity, light3D);
            }
        }
    }
};

class GameApp : public Engine::Application {
  public:
    GameApp() { addLayer<AppLayer>("game_area"); }

    virtual ~GameApp() {}
};

Engine::Application *Engine::createApplication() { return new GameApp(); }
