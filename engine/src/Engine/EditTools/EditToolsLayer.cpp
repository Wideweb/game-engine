#include "EditToolsLayer.hpp"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp>

#include "Application.hpp"
#include "Input.hpp"
#include "Mesh2D.hpp"
#include "TextureLoader.hpp"

namespace Engine {

Layer &EditToolsLayer::gameLayer() { return Application::get().getLayer("game_area"); }

void EditToolsLayer::onAttach() {
    Application::get().getTime().stop();
    Application::get().getCamera().setPosition(glm::vec3(8.0f, 6.0f, 8.0f));
    Application::get().getCamera().setRotation(glm::vec3(glm::radians(-25.0f), glm::radians(45.0f), 0.0f));

    auto &eventHandler = Application::get().getEventHandler();
    eventHandler.add<ApplicationError>([this](const ApplicationError &e) {
        m_Logs += e.what();
        m_Logs += "\n";
    });

    m_Imgui.OnAttach();

    m_ContentBrowserPanel = std::make_unique<ContentBrowserPanel>("assets");
    m_ContentBrowserPanel->onAttach();
    m_ContentBrowserPanel->show();

    m_SceneHierarchyPanel = std::make_unique<SceneHierarchyPanel>(m_GameObject);
    m_SceneHierarchyPanel->onAttach();
    m_SceneHierarchyPanel->show();

    m_TransformControls = std::make_unique<TransformControls>(m_GameObject);
    m_TransformControls->onAttach();
    m_TransformControls->hide();

    m_TransformPanel = std::make_unique<TransformPanel>(m_GameObject);
    m_TransformPanel->onAttach();
    m_TransformPanel->hide();

    m_ParticlesPanel = std::make_unique<ParticlesPanel>(m_GameObject);
    m_ParticlesPanel->onAttach();

    m_DirectedLightPanel = std::make_unique<DirectedLightPanel>(m_DirectedLightModel);
    m_DirectedLightPanel->onAttach();

    m_MaterialPanel = std::make_unique<MaterialPanel>(m_GameObject);
    m_MaterialPanel->onAttach();
    m_MaterialPanel->hide();

    m_BehaviourPanel = std::make_unique<BehaviourPanel>(m_GameObject);
    m_BehaviourPanel->onAttach();
    m_BehaviourPanel->hide();

    m_CameraDirector = std::make_unique<CameraDirector>(m_GameObject);
    m_CameraDirector->onAttach();

    m_GamePanel = std::make_unique<GamePanel>();
    m_GamePanel->onAttach();
}

void EditToolsLayer::onUpdate() {
    m_Imgui.onUpdate();
    m_GamePanel->onUpdate();

    if (!Application::get().getTime().poused()) {
        m_GameObject.setEntity(c_NoEntity);
    }

    if (m_GameObject.isActive()) {
        m_TransformControls->show();
        m_TransformPanel->show();
        m_BehaviourPanel->show();
    } else {
        m_TransformControls->hide();
        m_TransformPanel->hide();
        m_BehaviourPanel->hide();
    }

    if (m_GameObject.isActive() && m_GameObject.hasMaterial()) {
        m_MaterialPanel->show();
    } else {
        m_MaterialPanel->hide();
    }

    if (m_GameObject.isActive() && m_GameObject.hasParticles()) {
        m_ParticlesPanel->show();
    } else {
        m_ParticlesPanel->hide();
    }

    if (m_TransformControls->isVisible()) {
        m_TransformControls->onUpdate();
    }

    if (m_TransformPanel->isVisible()) {
        m_TransformPanel->onUpdate();
    }

    if (m_MaterialPanel->isVisible()) {
        m_MaterialPanel->onUpdate();
    }

    if (m_ParticlesPanel->isVisible()) {
        m_ParticlesPanel->onUpdate();
    }

    if (m_DirectedLightModel.isActive()) {
        m_DirectedLightPanel->show();
        m_DirectedLightPanel->onUpdate();
    } else {
        m_DirectedLightPanel->hide();
    }

    if (Application::get().getTime().poused()) {
        m_CameraDirector->show();
    } else {
        m_CameraDirector->hide();
    }

    if (m_CameraDirector->isVisible()) {
        m_CameraDirector->onUpdate();
    }

    if (Application::get().getTime().poused()) {
        auto &input = Application::get().getInput();
        auto &camera = Application::get().getCamera();
        if (input.IsKeyPressed(KeyCode::W)) {
            camera.move(glm::vec3(0.1f, 0.0f, 0.0f));
        }

        if (input.IsKeyPressed(KeyCode::S)) {
            camera.move(glm::vec3(-0.1f, 0.0f, 0.0f));
        }

        if (input.IsKeyPressed(KeyCode::A)) {
            camera.move(glm::vec3(0.0f, 0.0f, -0.1f));
        }

        if (input.IsKeyPressed(KeyCode::D)) {
            camera.move(glm::vec3(0.0f, 0.0f, 0.1f));
        }

        if (input.IsKeyPressed(KeyCode::Q)) {
            camera.move(glm::vec3(0.0f, -0.1f, 0.0f));
        }

        if (input.IsKeyPressed(KeyCode::E)) {
            camera.move(glm::vec3(0.0f, 0.1f, 0.0f));
        }

        if (input.IsKeyPressed(KeyCode::Backspace) && m_GameObject.isActive()) {
            gameLayer().getCoordinator().DestroyEntity(m_GameObject.entity());
            m_GameObject.setEntity(c_NoEntity);
        }

        if (input.IsKeyPressed(KeyCode::Space) && m_GameObject.isActive()) {
            glm::vec3 distance = camera.positionVec() - m_GameObject.position();
            glm::vec3 newPosition = m_GameObject.position() + glm::quat(glm::vec3(0.0f, 0.01f, 0.0f)) * distance;
            camera.setPosition(newPosition);
            camera.rotate(glm::vec3(0.0f, 0.01f, 0.0f));
        }
    }
}

void EditToolsLayer::onDraw() {
    const auto &window = Application::get().getWindow();

    int dWidth, dHeight;
    window.getDrawableSize(dWidth, dHeight);

    m_Imgui.Begin();

    ImGui::Begin("Console");
    ImGui::Text(m_Logs.c_str());
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);
    ImGui::End();

    m_GamePanel->onDraw(0, 0);
    m_CameraDirector->onDraw(0, 0);

    if (m_ContentBrowserPanel->isVisible()) {
        m_ContentBrowserPanel->onDraw(0, 0);
    }

    if (m_SceneHierarchyPanel->isVisible()) {
        m_SceneHierarchyPanel->onDraw(0, 0);
    }

    if (m_TransformControls->isVisible()) {
        m_TransformControls->onDraw(0, 0);
    }

    if (m_TransformPanel->isVisible()) {
        m_TransformPanel->onDraw(0, 250);
    }

    if (m_BehaviourPanel->isVisible()) {
        m_BehaviourPanel->onDraw(0, 250);
    }

    if (m_MaterialPanel->isVisible() && m_GameObject.hasMaterial()) {
        m_MaterialPanel->onDraw(0, 250);
    }

    if (m_ParticlesPanel->isVisible() && m_GameObject.hasParticles()) {
        m_ParticlesPanel->onDraw(0, 450);
    }

    if (m_DirectedLightModel.isActive()) {
        m_DirectedLightPanel->onDraw(dWidth - 150, 0);
    }

    m_Imgui.End();
}

void EditToolsLayer::onDetach() {
    m_GamePanel->onDetach();
    m_CameraDirector->onDetach();
    m_TransformControls->onDetach();
    m_TransformPanel->onDetach();
    m_ParticlesPanel->onDetach();
    m_DirectedLightPanel->onDetach();
    m_MaterialPanel->onDetach();
    m_BehaviourPanel->onDetach();
    m_Imgui.OnDetach();
}

void EditToolsLayer::onMouseEvent(MouseEvent &event) {
    m_Imgui.onMouseEvent(event);
    if (ImGui::GetIO().WantCaptureMouse && m_Imgui.BlockEvents()) {
        return;
    }

    if (m_TransformControls->isVisible()) {
        m_TransformControls->onMouseEvent(event);
    }

    if (event.type == EventType::MouseDown) {
        handleSelection();
    }

    if (!event.handled && Application::get().getTime().poused() &&
        Application::get().getInput().IsMousePressed(MouseButton::Left)) {
        auto mousePos = Application::get().getInput().GetMousePosition();

        glm::vec2 offset = (mousePos - m_MousePos) * 0.002f;

        Application::get().getCamera().rotate(glm::vec3(offset.y, offset.x, 0.0f));
    }
    m_MousePos = Application::get().getInput().GetMousePosition();
}

void EditToolsLayer::handleSelection() {
    auto ray = Application::get().getMousePicker().ray();
    auto pos = Application::get().getCamera().positionVec();
    auto editToolsEntities = m_Collision.Raycast(pos, ray, 100.0f);
    auto gameEntities = gameLayer().getCollision().Raycast(pos, ray, 100.0f);

    if (editToolsEntities.empty() && m_Imgui.entity() == c_NoEntity) {
        m_GameObject.setEntity(c_NoEntity);
    }

    if (!editToolsEntities.empty()) {
        Entity entity = editToolsEntities[0].id;

        if (m_TransformControls->isVisible()) {
            m_TransformControls->handleSelection(entity);
            return;
        }
    }

    if (m_Imgui.entity() != c_NoEntity) {
        m_GameObject.setEntity(m_Imgui.entity());
    }
}

} // namespace Engine
