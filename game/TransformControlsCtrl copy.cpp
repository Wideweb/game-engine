// #include "TransformControls.hpp"

// #include "InstancedModel.hpp"

// #include <glm/gtx/quaternion.hpp>
// #include <glm/gtx/transform.hpp>
// #include <glm/vec3.hpp>
// #include <memory>
// #include <string>
// #include <utility>

// #include <iostream>

// #include "imgui/imgui.h"

// TransformControls::TransformControls() {}

// Engine::Layer &TransformControls::gameLayer() { return Engine::Application::get().getLayer("game_area"); }
// Engine::Layer &TransformControls::toolsLayer() { return Engine::Application::get().getLayer("edit_tools"); }

// void TransformControls::onAttach() {
//     auto &coordinator = toolsLayer().getCoordinator();

//     Engine::Application::get().getModels().RegisterModel(
//         "arrow-x", Engine::ModelLoader::loadModel("./assets/models/box/arrow-x.fbx"));

//     Engine::Application::get().getModels().RegisterModel(
//         "arrow-y", Engine::ModelLoader::loadModel("./assets/models/box/arrow-y.fbx"));

//     Engine::Application::get().getModels().RegisterModel(
//         "arrow-z", Engine::ModelLoader::loadModel("./assets/models/box/arrow-z.fbx"));

//     auto arrowX = coordinator.CreateEntity("arrow-x");
//     auto arrowXLocation = Engine::LocationComponent();
//     arrowXLocation.rotation = glm::vec3(0.0f, 1.57f, 0.0f);
//     coordinator.AddComponent(arrowX, arrowXLocation);
//     coordinator.AddComponent(arrowX, Engine::Render3DComponent("arrow-x", 0.1f, true));
//     coordinator.AddComponent(arrowX, Engine::StaticCollisionComponent(0.5, 0.5, 0.5));
//     coordinator.AddComponent(arrowX, Engine::TagComponent("arrow"));
//     m_ArrowX = arrowX;

//     auto arrowY = coordinator.CreateEntity("arrow-y");
//     auto arrowYLocation = Engine::LocationComponent();
//     arrowYLocation.rotation = glm::vec3(-1.57f, 0.0f, 0.0f);
//     coordinator.AddComponent(arrowY, arrowYLocation);
//     coordinator.AddComponent(arrowY, Engine::Render3DComponent("arrow-y", 0.1f, true));
//     coordinator.AddComponent(arrowY, Engine::StaticCollisionComponent(0.5, 0.5, 0.5));
//     coordinator.AddComponent(arrowY, Engine::TagComponent("arrow"));
//     m_ArrowY = arrowY;

//     auto arrowZ = coordinator.CreateEntity("arrow-z");
//     auto arrowZLocation = Engine::LocationComponent();
//     arrowZLocation.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
//     coordinator.AddComponent(arrowZ, arrowZLocation);
//     coordinator.AddComponent(arrowZ, Engine::Render3DComponent("arrow-z", 0.1f, true));
//     coordinator.AddComponent(arrowZ, Engine::StaticCollisionComponent(0.5, 0.5, 0.5));
//     coordinator.AddComponent(arrowZ, Engine::TagComponent("arrow"));
//     m_ArrowZ = arrowZ;

//     Engine::Application::get().getModels().RegisterModel("arc-x",
//                                                          Engine::ModelLoader::loadModel("./assets/models/box/arc.obj"));
//     {
//         auto arcXModel = Engine::Application::get().getModels().GetModel<Engine::InstancedModel>("arc-x");
//         auto &vertices = arcXModel->meshes[0].vertices;
//         for (auto &vertex : vertices) {
//             vertex.color = glm::vec3(1.0f, 0.0f, 0.0f);
//         }
//         arcXModel->update();
//     }

//     Engine::Application::get().getModels().RegisterModel("arc-y",
//                                                          Engine::ModelLoader::loadModel("./assets/models/box/arc.obj"));
//     {
//         auto arcYModel = Engine::Application::get().getModels().GetModel<Engine::InstancedModel>("arc-y");
//         auto &vertices = arcYModel->meshes[0].vertices;
//         for (auto &vertex : vertices) {
//             vertex.color = glm::vec3(0.0f, 1.0f, 0.0f);
//         }
//         arcYModel->update();
//     }

//     Engine::Application::get().getModels().RegisterModel("arc-z",
//                                                          Engine::ModelLoader::loadModel("./assets/models/box/arc.obj"));
//     {
//         auto arcZModel = Engine::Application::get().getModels().GetModel<Engine::InstancedModel>("arc-z");
//         auto &vertices = arcZModel->meshes[0].vertices;
//         for (auto &vertex : vertices) {
//             vertex.color = glm::vec3(0.0f, 0.0f, 1.0f);
//         }
//         arcZModel->update();
//     }

//     auto arcX = coordinator.CreateEntity("arc-x");
//     auto arcXRender = Engine::Render3DComponent("arc-x", 0.1f, true);
//     arcXRender.rotation = glm::vec3(1.57f, 1.57f, 0.0f);
//     coordinator.AddComponent(arcX, arcXRender);
//     coordinator.AddComponent(arcX, Engine::LocationComponent());
//     coordinator.AddComponent(arcX, Engine::StaticCollisionComponent(0.25, 0.25, 0.25));
//     coordinator.AddComponent(arcX, Engine::TagComponent("arc"));
//     m_ArcX = arcX;

//     auto arcY = coordinator.CreateEntity("arc-y");
//     auto arcYRender = Engine::Render3DComponent("arc-y", 0.1f, true);
//     arcYRender.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
//     coordinator.AddComponent(arcY, arcYRender);
//     coordinator.AddComponent(arcY, Engine::LocationComponent());
//     coordinator.AddComponent(arcY, Engine::StaticCollisionComponent(0.25, 0.25, 0.25));
//     coordinator.AddComponent(arcY, Engine::TagComponent("arc"));
//     m_ArcY = arcY;

//     auto arcZ = coordinator.CreateEntity("arc-z");
//     auto arcZRender = Engine::Render3DComponent("arc-z", 0.1f, true);
//     arcZRender.rotation = glm::vec3(-1.57f, 0.0f, 0.0f);
//     coordinator.AddComponent(arcZ, arcZRender);
//     coordinator.AddComponent(arcZ, Engine::LocationComponent());
//     coordinator.AddComponent(arcZ, Engine::StaticCollisionComponent(0.25, 0.25, 0.25));
//     coordinator.AddComponent(arcZ, Engine::TagComponent("arc"));
//     m_ArcZ = arcZ;

//     {
//         Engine::Application::get().getModels().RegisterModel(
//             "scale-x", Engine::ModelLoader::loadModel("./assets/models/box/scale.obj"));

//         auto model = Engine::Application::get().getModels().GetModel<Engine::InstancedModel>("scale-x");
//         auto &vertices = model->meshes[0].vertices;
//         for (auto &vertex : vertices) {
//             vertex.color = glm::vec3(1.0f, 0.0f, 0.0f);
//         }
//         model->update();
//     }

//     {
//         Engine::Application::get().getModels().RegisterModel(
//             "scale-y", Engine::ModelLoader::loadModel("./assets/models/box/scale.obj"));

//         auto model = Engine::Application::get().getModels().GetModel<Engine::InstancedModel>("scale-y");
//         auto &vertices = model->meshes[0].vertices;
//         for (auto &vertex : vertices) {
//             vertex.color = glm::vec3(0.0f, 1.0f, 0.0f);
//         }
//         model->update();
//     }

//     {
//         Engine::Application::get().getModels().RegisterModel(
//             "scale-z", Engine::ModelLoader::loadModel("./assets/models/box/scale.obj"));

//         auto model = Engine::Application::get().getModels().GetModel<Engine::InstancedModel>("scale-z");
//         auto &vertices = model->meshes[0].vertices;
//         for (auto &vertex : vertices) {
//             vertex.color = glm::vec3(0.0f, 0.0f, 1.0f);
//         }
//         model->update();
//     }

//     {
//         auto modelAxisX = coordinator.CreateEntity("modelAxis-x");
//         auto modelAxisXRender = Engine::Render3DComponent("scale-x", 0.1f, true);
//         modelAxisXRender.rotation = glm::vec3(0.0f, 0.0f, -1.57f);
//         coordinator.AddComponent(modelAxisX, modelAxisXRender);
//         coordinator.AddComponent(modelAxisX, Engine::LocationComponent());
//         coordinator.AddComponent(modelAxisX, Engine::StaticCollisionComponent(0.5, 0.5, 0.5));
//         coordinator.AddComponent(modelAxisX, Engine::TagComponent("modelAxis"));
//         m_ModelAxisX = modelAxisX;

//         auto modelAxisY = coordinator.CreateEntity("modelAxis-y");
//         auto modelAxisYRender = Engine::Render3DComponent("scale-y", 0.1f, true);
//         modelAxisYRender.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
//         coordinator.AddComponent(modelAxisY, modelAxisYRender);
//         coordinator.AddComponent(modelAxisY, Engine::LocationComponent());
//         coordinator.AddComponent(modelAxisY, Engine::StaticCollisionComponent(0.5, 0.5, 0.5));
//         coordinator.AddComponent(modelAxisY, Engine::TagComponent("modelAxis"));
//         m_ModelAxisY = modelAxisY;

//         auto modelAxisZ = coordinator.CreateEntity("modelAxis-z");
//         auto modelAxisZRender = Engine::Render3DComponent("scale-z", 0.1f, true);
//         modelAxisZRender.rotation = glm::vec3(1.57f, 0.0f, 0.0f);
//         coordinator.AddComponent(modelAxisZ, modelAxisZRender);
//         coordinator.AddComponent(modelAxisZ, Engine::LocationComponent());
//         coordinator.AddComponent(modelAxisZ, Engine::StaticCollisionComponent(0.5, 0.5, 0.5));
//         coordinator.AddComponent(modelAxisZ, Engine::TagComponent("modelAxis"));
//         m_ModelAxisZ = modelAxisZ;
//     }

//     hide();
// }

// void TransformControls::onUpdate() {
//     auto &coordinator = toolsLayer().getCoordinator();
//     auto &gameCordinator = gameLayer().getCoordinator();
//     auto &location = gameCordinator.GetComponent<Engine::LocationComponent>(m_Target);
//     auto &collision = gameCordinator.GetComponent<Engine::StaticCollisionComponent>(m_Target);
//     auto &render = gameCordinator.GetComponent<Engine::Render3DComponent>(m_Target);

//     std::vector<glm::vec3> vertices;
//     vertices.reserve(8);
//     std::transform(collision.vertices.begin(), collision.vertices.end(), std::back_inserter(vertices),
//                    [&](const glm::vec3 &v) { return glm::quat(location.rotation) * v * render.scale; });
//     collision.vertices = std::move(vertices);

//     collision.updated = true;
//     location.updated = true;
//     collision.updated = true;
// }

// void TransformControls::onDraw(int x, int y) {
//     auto &gameCordinator = gameLayer().getCoordinator();
//     auto &coordinator = toolsLayer().getCoordinator();
//     auto &location = gameCordinator.GetComponent<Engine::LocationComponent>(m_Target);
//     auto &render = gameCordinator.GetComponent<Engine::Render3DComponent>(m_Target);
//     auto &collision = gameCordinator.GetComponent<Engine::StaticCollisionComponent>(m_Target);

//     ImGui::SetNextWindowSize(ImVec2(200, 270));
//     ImGui::SetNextWindowPos(ImVec2(x, y));

//     ImGui::Begin("Transform");

//     {
//         ImGui::Text("Location: ");
//         ImGui::InputFloat("x", &location.position.x, 0.1f, 0.01f);
//         ImGui::InputFloat("y", &location.position.y, 0.1f, 0.01f);
//         ImGui::InputFloat("z", &location.position.z, 0.1f, 0.01f);
//     }

//     {
//         ImGui::Text("Rotation: ");
//         ImGui::InputFloat("x", &location.rotation.x, 0.1f, 0.01f);
//         ImGui::InputFloat("y", &location.rotation.y, 0.1f, 0.01f);
//         ImGui::InputFloat("z", &location.rotation.z, 0.1f, 0.01f);
//     }

//     {
//         ImGui::Text("Scale: ");
//         ImGui::InputFloat("", &render.scale, 0.1f, 0.01f);
//     }

//     // ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

//     ImGui::End();

//     location.updated = true;
//     collision.updated = true;

//     {
//         auto &renderX = coordinator.GetComponent<Engine::Render3DComponent>(m_ArrowX);
//         auto &renderY = coordinator.GetComponent<Engine::Render3DComponent>(m_ArrowY);
//         auto &renderZ = coordinator.GetComponent<Engine::Render3DComponent>(m_ArrowZ);

//         renderX.scale = 0.1f;
//         renderY.scale = 0.1f;
//         renderZ.scale = 0.1f;

//         renderX.updated = true;
//         renderY.updated = true;
//         renderZ.updated = true;

//         auto &arrowXLocation = coordinator.GetComponent<Engine::LocationComponent>(m_ArrowX);
//         auto &arrowYLocation = coordinator.GetComponent<Engine::LocationComponent>(m_ArrowY);
//         auto &arrowZLocation = coordinator.GetComponent<Engine::LocationComponent>(m_ArrowZ);

//         arrowXLocation.position = location.position + glm::vec3(2.0f, 0.0f, 0.0f);
//         arrowYLocation.position = location.position + glm::vec3(0.0f, 2.0f, 0.0f);
//         arrowZLocation.position = location.position + glm::vec3(0.0f, 0.0f, 2.0f);

//         arrowXLocation.updated = true;
//         arrowYLocation.updated = true;
//         arrowZLocation.updated = true;

//         auto &collisionX = coordinator.GetComponent<Engine::StaticCollisionComponent>(m_ArrowX);
//         auto &collisionY = coordinator.GetComponent<Engine::StaticCollisionComponent>(m_ArrowY);
//         auto &collisionZ = coordinator.GetComponent<Engine::StaticCollisionComponent>(m_ArrowZ);

//         collisionX.updated = true;
//         collisionY.updated = true;
//         collisionZ.updated = true;
//     }

//     {
//         auto &renderX = coordinator.GetComponent<Engine::Render3DComponent>(m_ArcX);
//         auto &renderY = coordinator.GetComponent<Engine::Render3DComponent>(m_ArcY);
//         auto &renderZ = coordinator.GetComponent<Engine::Render3DComponent>(m_ArcZ);

//         renderX.scale = 0.1f;
//         renderY.scale = 0.1f;
//         renderZ.scale = 0.1f;

//         renderX.updated = true;
//         renderY.updated = true;
//         renderZ.updated = true;

//         auto &arcXLocation = coordinator.GetComponent<Engine::LocationComponent>(m_ArcX);
//         auto &arcYLocation = coordinator.GetComponent<Engine::LocationComponent>(m_ArcY);
//         auto &arcZLocation = coordinator.GetComponent<Engine::LocationComponent>(m_ArcZ);

//         arcXLocation.position = location.position + glm::vec3(1.5f, 0.5f, 0.0f);
//         arcYLocation.position = location.position + glm::vec3(0.0f, 1.5f, -0.5f);
//         arcZLocation.position = location.position + glm::vec3(0.0f, -0.5f, 1.5f);

//         arcXLocation.updated = true;
//         arcYLocation.updated = true;
//         arcZLocation.updated = true;

//         auto &collisionX = coordinator.GetComponent<Engine::StaticCollisionComponent>(m_ArcX);
//         auto &collisionY = coordinator.GetComponent<Engine::StaticCollisionComponent>(m_ArcY);
//         auto &collisionZ = coordinator.GetComponent<Engine::StaticCollisionComponent>(m_ArcZ);

//         collisionX.updated = true;
//         collisionY.updated = true;
//         collisionZ.updated = true;
//     }

//     {
//         auto &renderX = coordinator.GetComponent<Engine::Render3DComponent>(m_ModelAxisX);
//         auto &renderY = coordinator.GetComponent<Engine::Render3DComponent>(m_ModelAxisY);
//         auto &renderZ = coordinator.GetComponent<Engine::Render3DComponent>(m_ModelAxisZ);

//         renderX.scale = 0.05f;
//         renderY.scale = 0.05f;
//         renderZ.scale = 0.05f;

//         renderX.updated = true;
//         renderY.updated = true;
//         renderZ.updated = true;

//         auto &locationX = coordinator.GetComponent<Engine::LocationComponent>(m_ModelAxisX);
//         auto &locationY = coordinator.GetComponent<Engine::LocationComponent>(m_ModelAxisY);
//         auto &locationZ = coordinator.GetComponent<Engine::LocationComponent>(m_ModelAxisZ);

//         glm::quat rot = glm::quat(location.rotation) * glm::quat(render.rotation);
//         glm::vec3 rotEulerAngles = glm::eulerAngles(glm::quat(location.rotation) * glm::quat(render.rotation));

//         locationX.rotation = rotEulerAngles;
//         locationY.rotation = rotEulerAngles;
//         locationZ.rotation = rotEulerAngles;

//         locationX.position = location.position + rot * glm::vec3(0.35f, 0.0f, 0.0f);
//         locationY.position = location.position + rot * glm::vec3(0.0f, 0.35f, 0.0f);
//         locationZ.position = location.position + rot * glm::vec3(0.0f, 0.0f, 0.35f);

//         locationX.updated = true;
//         locationY.updated = true;
//         locationZ.updated = true;

//         auto &collisionX = coordinator.GetComponent<Engine::StaticCollisionComponent>(m_ModelAxisX);
//         auto &collisionY = coordinator.GetComponent<Engine::StaticCollisionComponent>(m_ModelAxisY);
//         auto &collisionZ = coordinator.GetComponent<Engine::StaticCollisionComponent>(m_ModelAxisZ);

//         collisionX.updated = true;
//         collisionY.updated = true;
//         collisionZ.updated = true;
//     }
// }

// void TransformControls::onDetach() {}

// void TransformControls::onMouseEvent(Engine::MouseEvent &event) {
//     if (event.type == Engine::EventType::MouseMoved) {
//         if (m_ActiveControl != Engine::c_NoEntity && m_Target != Engine::c_NoEntity) {
//             onTransform();
//         }
//     }

//     if (event.type == Engine::EventType::MouseUp) {
//         m_ActiveControl = Engine::c_NoEntity;
//     }

//     m_PrevMouseWorldPos = Engine::Application::get().getMousePicker().ray();
// }

// bool TransformControls::handleSelection(Engine::Entity entity) {
//     if (entity != m_ArrowX && entity != m_ArrowY && entity != m_ArrowZ && entity != m_ArcX && entity != m_ArcY &&
//         entity != m_ArcZ && entity != m_ModelAxisX && entity != m_ModelAxisY && entity != m_ModelAxisZ) {
//         return false;
//     }

//     m_ActiveControl = entity;
//     m_InitMouseScreenPos = Engine::Application::get().getInput().GetMousePosition();
//     m_InitScale = gameLayer().getCoordinator().GetComponent<Engine::Render3DComponent>(m_Target).scale;
//     return true;
// }

// void TransformControls::setTarget(Engine::Entity entity) { m_Target = entity; }

// void TransformControls::show() { m_isVisible = true; }

// void TransformControls::hide() {
//     auto &coordinator = toolsLayer().getCoordinator();

//     {
//         auto &renderX = coordinator.GetComponent<Engine::Render3DComponent>(m_ArrowX);
//         auto &renderY = coordinator.GetComponent<Engine::Render3DComponent>(m_ArrowY);
//         auto &renderZ = coordinator.GetComponent<Engine::Render3DComponent>(m_ArrowZ);

//         renderX.scale = 0.0f;
//         renderY.scale = 0.0f;
//         renderZ.scale = 0.0f;

//         renderX.updated = true;
//         renderY.updated = true;
//         renderZ.updated = true;
//     }

//     {
//         auto &renderX = coordinator.GetComponent<Engine::Render3DComponent>(m_ArcX);
//         auto &renderY = coordinator.GetComponent<Engine::Render3DComponent>(m_ArcY);
//         auto &renderZ = coordinator.GetComponent<Engine::Render3DComponent>(m_ArcZ);

//         renderX.scale = 0.0f;
//         renderY.scale = 0.0f;
//         renderZ.scale = 0.0f;

//         renderX.updated = true;
//         renderY.updated = true;
//         renderZ.updated = true;
//     }

//     {
//         auto &renderX = coordinator.GetComponent<Engine::Render3DComponent>(m_ModelAxisX);
//         auto &renderY = coordinator.GetComponent<Engine::Render3DComponent>(m_ModelAxisY);
//         auto &renderZ = coordinator.GetComponent<Engine::Render3DComponent>(m_ModelAxisZ);

//         renderX.scale = 0.0f;
//         renderY.scale = 0.0f;
//         renderZ.scale = 0.0f;

//         renderX.updated = true;
//         renderY.updated = true;
//         renderZ.updated = true;
//     }

//     m_isVisible = false;
// }

// bool TransformControls::isVisible() { return m_isVisible; }

// void TransformControls::onTransform() {
//     auto mousePos = Engine::Application::get().getMousePicker().ray();
//     auto pos = Engine::Application::get().getCamera().positionVec();

//     auto &coordinator = toolsLayer().getCoordinator();
//     auto &gameCordinator = gameLayer().getCoordinator();
//     auto &location = gameCordinator.GetComponent<Engine::LocationComponent>(m_Target);
//     auto &collision = gameCordinator.GetComponent<Engine::StaticCollisionComponent>(m_Target);
//     auto &render = gameCordinator.GetComponent<Engine::Render3DComponent>(m_Target);

//     glm::vec3 dPosition = (mousePos - m_PrevMouseWorldPos) * glm::distance(pos, location.position);
//     glm::vec3 dRotation = dPosition;
//     float scale = 1.0f;

//     if (m_ActiveControl != m_ArrowX) {
//         dPosition.x = 0;
//     }
//     if (m_ActiveControl != m_ArrowY) {
//         dPosition.y = 0;
//     }
//     if (m_ActiveControl != m_ArrowZ) {
//         dPosition.z = 0;
//     }

//     if (m_ActiveControl == m_ModelAxisX || m_ActiveControl == m_ModelAxisY || m_ActiveControl == m_ModelAxisZ) {
//         auto &camera = Engine::Application::get().getCamera();
//         glm::vec4 screenPos = camera.projectionMatrix() * camera.viewMatrix() * glm::vec4(location.position, 1.0f);
//         screenPos /= screenPos.w;
//         screenPos.y *= -1.0f;
//         screenPos = (screenPos + 1.0f) / 2.0f;

//         glm::vec2 initMousePos = m_InitMouseScreenPos;
//         initMousePos /= Engine::Application::get().getWindow().getSize();

//         glm::vec2 curMousePos = Engine::Application::get().getInput().GetMousePosition();
//         curMousePos /= Engine::Application::get().getWindow().getSize();

//         float initDist = glm::distance(glm::vec2(screenPos), initMousePos);
//         float currDist = glm::distance(glm::vec2(screenPos), curMousePos);

//         float prevScale = render.scale / m_InitScale;
//         float newScale = currDist / initDist;
//         scale = m_InitScale / prevScale * newScale;
//     }

//     if (m_ActiveControl == m_ArcX) {
//         dRotation.x = -dRotation.y + dRotation.z;
//         dRotation.y = 0;
//         dRotation.z = 0;
//     } else if (m_ActiveControl == m_ArcY) {
//         dRotation.y = -(dRotation.x + dRotation.z);
//         dRotation.x = 0;
//         dRotation.z = 0;
//     } else if (m_ActiveControl == m_ArcZ) {
//         dRotation.z = dRotation.x + -dRotation.y;
//         dRotation.x = 0;
//         dRotation.y = 0;
//     } else {
//         dRotation = glm::vec3(0.0f);
//     }

//     location.position = location.position + dPosition;
//     location.rotation = glm::eulerAngles(glm::quat(dRotation) * glm::quat(location.rotation));
//     render.scale = scale;
// }