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

    Application::get().getTextures().add("engine_placeholder", TextureLoader::placeholder());

    auto &eventHandler = Application::get().getEventHandler();
    eventHandler.add<ApplicationError>([this](const ApplicationError &e) {
        m_Logs += e.what();
        m_Logs += "\n";
    });

    m_Imgui.OnAttach();

    m_RenderPanel = std::make_unique<RenderPanel>();
    m_RenderPanel->onAttach();
    m_RenderPanel->show();

    m_ContentBrowserPanel = std::make_unique<ContentBrowserPanel>("assets");
    m_ContentBrowserPanel->onAttach();
    m_ContentBrowserPanel->show();

    m_SceneHierarchyPanel = std::make_unique<SceneHierarchyPanel>(m_GameObject);
    m_SceneHierarchyPanel->onAttach();
    m_SceneHierarchyPanel->show();

    m_TransformPanel = std::make_unique<TransformPanel>(m_GameObject);
    m_TransformPanel->onAttach();
    m_TransformPanel->hide();

    m_TransformControlsPosition = std::make_unique<TransformControlsPosition>(m_GameObject);
    m_TransformControlsPosition->onAttach();
    m_TransformControlsPosition->hide();

    m_TransformControlsRotation = std::make_unique<TransformControlsRotation>(m_GameObject);
    m_TransformControlsRotation->onAttach();
    m_TransformControlsRotation->hide();

    m_TransformControlsScale = std::make_unique<TransformControlsScale>(m_GameObject);
    m_TransformControlsScale->onAttach();
    m_TransformControlsScale->hide();

    m_VelocityPanel = std::make_unique<VelocityPanel>(m_GameObject);
    m_VelocityPanel->onAttach();
    m_VelocityPanel->hide();

    m_ParticlesPanel = std::make_unique<ParticlesPanel>(m_GameObject);
    m_ParticlesPanel->onAttach();

    m_DirectedLightPanel = std::make_unique<DirectedLightPanel>(m_GameObject);
    m_DirectedLightPanel->onAttach();
    m_DirectedLightPanel->hide();

    m_MaterialPanel = std::make_unique<MaterialPanel>(m_GameObject);
    m_MaterialPanel->onAttach();
    m_MaterialPanel->hide();

    m_BehaviourPanel = std::make_unique<BehaviourPanel>(m_GameObject);
    m_BehaviourPanel->onAttach();
    m_BehaviourPanel->hide();

    m_SkeletPanel = std::make_unique<SkeletPanel>(m_GameObject);
    m_SkeletPanel->onAttach();
    m_SkeletPanel->hide();

    m_CameraDirector = std::make_unique<CameraDirector>(m_GameObject);
    m_CameraDirector->onAttach();

    m_GamePanel = std::make_unique<GamePanel>();
    m_GamePanel->onAttach();

    m_DirectedLightDirector = std::make_unique<DirectedLightDirector>(m_GameObject);
    m_DirectedLightDirector->onAttach();
}

void EditToolsLayer::onUpdate() {
    m_Imgui.onUpdate();
    m_GamePanel->onUpdate();

    if (m_RenderPanel->isVisible()) {
        m_RenderPanel->onUpdate();
    }

    if (!Application::get().getTime().poused()) {
        m_GameObject.setEntity(c_NoEntity);
    }

    if (m_GameObject.isActive()) {
        m_TransformPanel->show();
        m_BehaviourPanel->show();
    } else {
        m_TransformPanel->hide();
        m_BehaviourPanel->hide();
    }

    if (m_GameObject.isActive() && m_GameObject.hasPosition()) {
        m_TransformControlsPosition->show();
    } else {
        m_TransformControlsPosition->hide();
    }

    if (m_GameObject.isActive() && m_GameObject.hasRotation()) {
        m_TransformControlsRotation->show();
    } else {
        m_TransformControlsRotation->hide();
    }

    if (m_GameObject.isActive() && m_GameObject.hasScale()) {
        m_TransformControlsScale->show();
    } else {
        m_TransformControlsScale->hide();
    }

    if (m_GameObject.isActive() && m_GameObject.hasVelocity()) {
        m_VelocityPanel->show();
    } else {
        m_VelocityPanel->hide();
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

    if (m_GameObject.isActive() && m_GameObject.hasDirectedLight()) {
        m_DirectedLightPanel->show();
    } else {
        m_DirectedLightPanel->hide();
    }

    if (m_TransformControlsPosition->isVisible()) {
        m_TransformControlsPosition->onUpdate();
    }

    if (m_TransformControlsRotation->isVisible()) {
        m_TransformControlsRotation->onUpdate();
    }

    if (m_TransformControlsScale->isVisible()) {
        m_TransformControlsScale->onUpdate();
    }

    if (m_TransformPanel->isVisible()) {
        m_TransformPanel->onUpdate();
    }

    if (m_VelocityPanel->isVisible()) {
        m_VelocityPanel->onUpdate();
    }

    if (m_MaterialPanel->isVisible()) {
        m_MaterialPanel->onUpdate();
    }

    if (m_GameObject.isActive() && m_GameObject.hasSkelet()) {
        m_SkeletPanel->show();
    } else {
        m_SkeletPanel->hide();
    }

    if (m_ParticlesPanel->isVisible()) {
        m_ParticlesPanel->onUpdate();
    }

    if (m_DirectedLightPanel->isVisible()) {
        m_DirectedLightPanel->onUpdate();
    }

    if (Application::get().getTime().poused()) {
        m_CameraDirector->show();
        m_DirectedLightDirector->show();
    } else {
        m_CameraDirector->hide();
        m_DirectedLightDirector->hide();
    }

    if (m_CameraDirector->isVisible()) {
        m_CameraDirector->onUpdate();
    }

    if (m_DirectedLightDirector->isVisible()) {
        m_DirectedLightDirector->onUpdate();
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

    {
        bool cameraDirectorVisible = m_CameraDirector->isVisible();
        bool directedLightDirectorVisible = m_DirectedLightDirector->isVisible();

        if (cameraDirectorVisible) {
            m_CameraDirector->hide();
        }

        if (directedLightDirectorVisible) {
            m_DirectedLightDirector->hide();
        }

        m_GamePanel->onDraw(0, 0);

        if (cameraDirectorVisible) {
            m_CameraDirector->show();
        }

        if (directedLightDirectorVisible) {
            m_DirectedLightDirector->show();
        }
    }

    m_CameraDirector->onDraw(0, 0);
    m_DirectedLightDirector->onDraw(0, 0);

    if (m_RenderPanel->isVisible()) {
        m_RenderPanel->onDraw(0, 0);
    }

    if (m_ContentBrowserPanel->isVisible()) {
        m_ContentBrowserPanel->onDraw(0, 0);
    }

    if (m_SceneHierarchyPanel->isVisible()) {
        m_SceneHierarchyPanel->onDraw(0, 0);
    }

    if (m_TransformControlsPosition->isVisible()) {
        m_TransformControlsPosition->onDraw(0, 0);
    }

    if (m_TransformControlsRotation->isVisible()) {
        m_TransformControlsRotation->onDraw(0, 0);
    }

    if (m_TransformControlsScale->isVisible()) {
        m_TransformControlsScale->onDraw(0, 0);
    }

    if (m_TransformPanel->isVisible()) {
        m_TransformPanel->onDraw(0, 250);
    }

    if (m_VelocityPanel->isVisible()) {
        m_VelocityPanel->onDraw(0, 0);
    }

    if (m_BehaviourPanel->isVisible()) {
        m_BehaviourPanel->onDraw(0, 250);
    }

    if (m_MaterialPanel->isVisible()) {
        m_MaterialPanel->onDraw(0, 250);
    }

    if (m_SkeletPanel->isVisible()) {
        m_SkeletPanel->onDraw(0, 250);
    }

    if (m_ParticlesPanel->isVisible()) {
        m_ParticlesPanel->onDraw(0, 450);
    }

    if (m_DirectedLightPanel->isVisible()) {
        m_DirectedLightPanel->onDraw(0, 0);
    }

    m_Imgui.End();
}

void EditToolsLayer::onDetach() {
    m_GamePanel->onDetach();
    m_CameraDirector->onDetach();
    m_DirectedLightDirector->onDetach();
    m_TransformControlsPosition->onDetach();
    m_TransformControlsRotation->onDetach();
    m_TransformControlsScale->onDetach();
    m_TransformPanel->onDetach();
    m_VelocityPanel->onDetach();
    m_ParticlesPanel->onDetach();
    m_DirectedLightPanel->onDetach();
    m_MaterialPanel->onDetach();
    m_BehaviourPanel->onDetach();
    m_SkeletPanel->onDetach();
    m_Imgui.OnDetach();
}

void EditToolsLayer::onMouseEvent(MouseEvent &event) {
    m_Imgui.onMouseEvent(event);
    if (ImGui::GetIO().WantCaptureMouse && m_Imgui.BlockEvents()) {
        return;
    }

    if (m_TransformControlsPosition->isVisible()) {
        m_TransformControlsPosition->onMouseEvent(event);
        if (event.handled) {
            return;
        }
    }

    if (m_TransformControlsRotation->isVisible()) {
        m_TransformControlsRotation->onMouseEvent(event);
        if (event.handled) {
            return;
        }
    }

    if (m_TransformControlsScale->isVisible()) {
        m_TransformControlsScale->onMouseEvent(event);
        if (event.handled) {
            return;
        }
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

        if (m_TransformControlsPosition->isVisible() && m_TransformControlsPosition->handleSelection(entity)) {
            return;
        }

        if (m_TransformControlsRotation->isVisible() && m_TransformControlsRotation->handleSelection(entity)) {
            return;
        }

        if (m_TransformControlsScale->isVisible() && m_TransformControlsScale->handleSelection(entity)) {
            return;
        }
    }

    if (m_Imgui.entity() != c_NoEntity) {
        m_GameObject.setEntity(m_Imgui.entity());
    }
}

} // namespace Engine
