#include "EditToolsLayer.hpp"

#include <glm/vec3.hpp>
#include <memory>
#include <string>

// auto &render = m_Coordinator.GetComponent<Engine::Render3DComponent>(entityId);
// auto &collision = m_Coordinator.GetComponent<Engine::StaticCollisionComponent>(entityId);

// glm::mat4 revertModel = glm::inverse(glm::scale(glm::vec3(render.scale)));

// render.scale += 0.01f;
// render.updated = true;

// glm::mat4 model = glm::scale(glm::vec3(render.scale));
// for (auto &vertex : collision.vertices) {
//     vertex = glm::vec3(model * revertModel * glm::vec4(vertex, 1.0));
// }
// collision.updated = true;

Engine::Layer &EditToolsLayer::gameLayer() { return Engine::Application::get().getLayer("game_area"); }

void EditToolsLayer::onAttach() {
    Engine::Application::get().getModels().RegisterModel(
        "arrow-x", Engine::ModelLoader::loadModel("./assets/models/box/arrow-x.fbx"));

    Engine::Application::get().getModels().RegisterModel(
        "arrow-y", Engine::ModelLoader::loadModel("./assets/models/box/arrow-y.fbx"));

    Engine::Application::get().getModels().RegisterModel(
        "arrow-z", Engine::ModelLoader::loadModel("./assets/models/box/arrow-z.fbx"));

    auto arrowX = m_Coordinator.CreateEntity("arrow-x");
    auto arrowXLocation = Engine::LocationComponent();
    arrowXLocation.rotation = glm::vec3(0.0f, -1.57f, 0.0f);
    m_Coordinator.AddComponent(arrowX, arrowXLocation);
    m_Coordinator.AddComponent(arrowX, Engine::Render3DComponent("arrow-x", 0.1f, true));
    m_Coordinator.AddComponent(arrowX, Engine::StaticCollisionComponent(0.5, 0.5, 0.5));
    m_Coordinator.AddComponent(arrowX, Engine::TagComponent("arrow"));
    m_ArrowX = arrowX;

    auto arrowY = m_Coordinator.CreateEntity("arrow-y");
    auto arrowYLocation = Engine::LocationComponent();
    arrowYLocation.rotation = glm::vec3(-1.57f, 0.0f, 0.0f);
    m_Coordinator.AddComponent(arrowY, arrowYLocation);
    m_Coordinator.AddComponent(arrowY, Engine::Render3DComponent("arrow-y", 0.1f, true));
    m_Coordinator.AddComponent(arrowY, Engine::StaticCollisionComponent(0.5, 0.5, 0.5));
    m_Coordinator.AddComponent(arrowY, Engine::TagComponent("arrow"));
    m_ArrowY = arrowY;

    auto arrowZ = m_Coordinator.CreateEntity("arrow-z");
    auto arrowZLocation = Engine::LocationComponent();
    arrowZLocation.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    m_Coordinator.AddComponent(arrowZ, arrowZLocation);
    m_Coordinator.AddComponent(arrowZ, Engine::Render3DComponent("arrow-z", 0.1f, true));
    m_Coordinator.AddComponent(arrowZ, Engine::StaticCollisionComponent(0.5, 0.5, 0.5));
    m_Coordinator.AddComponent(arrowZ, Engine::TagComponent("arrow"));
    m_ArrowZ = arrowZ;
}

void EditToolsLayer::onUpdate() {
    if (m_ActiveEntity != Engine::c_NoEntity) {
        auto &location = gameLayer().getCoordinator().GetComponent<Engine::LocationComponent>(m_ActiveEntity);

        auto &renderX = m_Coordinator.GetComponent<Engine::Render3DComponent>(m_ArrowX);
        auto &renderY = m_Coordinator.GetComponent<Engine::Render3DComponent>(m_ArrowY);
        auto &renderZ = m_Coordinator.GetComponent<Engine::Render3DComponent>(m_ArrowZ);

        renderX.scale = 0.1f;
        renderY.scale = 0.1f;
        renderZ.scale = 0.1f;

        renderX.updated = true;
        renderY.updated = true;
        renderZ.updated = true;

        auto &arrowXLocation = m_Coordinator.GetComponent<Engine::LocationComponent>(m_ArrowX);
        auto &arrowYLocation = m_Coordinator.GetComponent<Engine::LocationComponent>(m_ArrowY);
        auto &arrowZLocation = m_Coordinator.GetComponent<Engine::LocationComponent>(m_ArrowZ);

        arrowXLocation.position = location.position + glm::vec3(2.0f, 0.0f, 0.0f);
        arrowYLocation.position = location.position + glm::vec3(0.0f, 2.0f, 0.0f);
        arrowZLocation.position = location.position + glm::vec3(0.0f, 0.0f, 2.0f);

        arrowXLocation.updated = true;
        arrowYLocation.updated = true;
        arrowZLocation.updated = true;
    } else {
        auto &renderX = m_Coordinator.GetComponent<Engine::Render3DComponent>(m_ArrowX);
        auto &renderY = m_Coordinator.GetComponent<Engine::Render3DComponent>(m_ArrowY);
        auto &renderZ = m_Coordinator.GetComponent<Engine::Render3DComponent>(m_ArrowZ);

        renderX.scale = 0.0f;
        renderY.scale = 0.0f;
        renderZ.scale = 0.0f;

        renderX.updated = true;
        renderY.updated = true;
        renderZ.updated = true;
    }
}

void EditToolsLayer::onDetach() {}

void EditToolsLayer::onMouseEvent(Engine::MouseEvent &event) {
    if (event.type == Engine::EventType::MouseMoved) {
        if (m_ActiveArrow != Engine::c_NoEntity && m_ActiveEntity != Engine::c_NoEntity) {
            auto mousePos = Engine::Application::get().getMousePicker().ray();
            auto pos = Engine::Application::get().getCamera().positionVec();

            auto &location = gameLayer().getCoordinator().GetComponent<Engine::LocationComponent>(m_ActiveEntity);

            glm::vec3 delta = (mousePos - m_PrevMousePosition) * glm::distance(pos, location.position);
            if (m_ActiveArrow != m_ArrowX) {
                delta.x = 0;
            }
            if (m_ActiveArrow != m_ArrowY) {
                delta.y = 0;
            }
            if (m_ActiveArrow != m_ArrowZ) {
                delta.z = 0;
            }

            location.position += delta;
            location.updated = true;
        }
    }

    if (event.type == Engine::EventType::MouseUp) {
        m_MousePressed = false;
        m_ActiveArrow = Engine::c_NoEntity;
    }

    if (event.type == Engine::EventType::MouseDown && !m_MousePressed) {

        auto ray = Engine::Application::get().getMousePicker().ray();
        auto pos = Engine::Application::get().getCamera().positionVec();
        auto result = m_Collision.Raycast(pos, ray, 100.0f);
        auto result2 = gameLayer().getCollision().Raycast(pos, ray, 100.0f);

        if (result.empty() && result2.empty()) {
            m_ActiveEntity = Engine::c_NoEntity;
        }

        if (!result.empty()) {
            for (auto &entity : result) {
                std::string tag = m_Coordinator.GetComponent<Engine::TagComponent>(entity.id).tag;
                if (tag == "arrow") {
                    m_ActiveArrow = entity.id;
                    break;
                }
            }
        }

        if (result.empty() && !result2.empty()) {
            Engine::Entity entityId = result2[0].id;
            if (gameLayer().getCoordinator().HasComponent<Engine::Render3DComponent>(entityId)) {
                std::string tag = gameLayer().getCoordinator().GetComponent<Engine::TagComponent>(entityId).tag;
                m_ActiveEntity = entityId;
            }
        }
    }

    m_PrevMousePosition = Engine::Application::get().getMousePicker().ray();
}
