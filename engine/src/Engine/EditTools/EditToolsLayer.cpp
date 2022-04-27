#include "EditToolsLayer.hpp"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp>
#include <limits>

#include "Application.hpp"
#include "EditToolComponent.hpp"
#include "ImGuiWidgets.hpp"
#include "Input.hpp"
#include "Mesh2D.hpp"
#include "TextureLoader.hpp"

namespace Engine {

Layer &EditToolsLayer::gameLayer() { return Application::get().getLayer("game_area"); }

void EditToolsLayer::onAttach() {
    Application::get().getTime().stop();
    Application::get().getCamera().setPosition(glm::vec3(8.0f, 6.0f, 8.0f));
    Application::get().getCamera().setRotation(glm::quat(glm::vec3(glm::radians(-25.0f), glm::radians(45.0f), 0.0f)));

    m_GameObject.entityChange$.addEventCallback([this](Entity entity) {
        if (Application::get().getTime().poused()) {
            ImGui::SetWindowFocus();
        }
    });

    m_Imgui.OnAttach();

    m_SceneTools[0] = {std::make_unique<CameraDirector>(m_GameObject),
                       [&]() { return Application::get().getTime().poused(); }};

    m_SceneTools[1] = {std::make_unique<DirectedLightDirector>(m_GameObject),
                       [&]() { return Application::get().getTime().poused(); }};

    for (auto &tool : m_SceneTools) {
        tool.view->onAttach();
        tool.view->hide();
    }

    m_ScenePanel = std::make_unique<ScenePanel>();
    m_ScenePanel->onAttach();
    m_ScenePanel->show();

    m_ToolbarPanel = std::make_unique<ToolbarPanel>();
    m_ToolbarPanel->onAttach();
    m_ToolbarPanel->show();

    m_GamePanel = std::make_unique<GamePanel>();
    m_GamePanel->onAttach();
    m_GamePanel->show();

    m_RenderPanel = std::make_unique<RenderPanel>();
    m_RenderPanel->onAttach();
    m_RenderPanel->show();

    m_ConsolePanel = std::make_unique<ConsolePanel>();
    m_ConsolePanel->onAttach();
    m_ConsolePanel->show();

    m_ContentBrowserPanel = std::make_unique<ContentBrowserPanel>("assets");
    m_ContentBrowserPanel->onAttach();
    m_ContentBrowserPanel->show();

    m_SceneHierarchyPanel = std::make_unique<SceneHierarchyPanel>(m_GameObject);
    m_SceneHierarchyPanel->onAttach();
    m_SceneHierarchyPanel->show();

    m_InspectorPanel = std::make_unique<InspectorPanel>(m_GameObject);
    m_InspectorPanel->onAttach();
    m_InspectorPanel->show();
}

void EditToolsLayer::onUpdate() {
    m_GameObject.update();

    if (Application::get().getTime().poused() && !ImGui::IsAnyItemActive()) {
        auto &input = Application::get().getInput();
        auto &camera = Application::get().getCamera();
        if (input.IsKeyPressed(KeyCode::W)) {
            glm::vec3 delta = glm::vec3(0.5, 0.0f, 0.0f);
            Application::get().getCameraController().move(delta, 0.1);
        }

        if (input.IsKeyPressed(KeyCode::S)) {
            glm::vec3 delta = glm::vec3(-0.5, 0.0f, 0.0f);
            Application::get().getCameraController().move(delta, 0.1);
        }

        if (input.IsKeyPressed(KeyCode::A)) {
            glm::vec3 delta = glm::vec3(0.0f, 0.0f, -0.5);
            Application::get().getCameraController().move(delta, 0.1);
        }

        if (input.IsKeyPressed(KeyCode::D)) {
            glm::vec3 delta = glm::vec3(0.0f, 0.0f, 0.5);
            Application::get().getCameraController().move(delta, 0.1);
        }

        if (input.IsKeyPressed(KeyCode::Q)) {
            glm::vec3 delta = glm::vec3(0.0f, -0.5, 0.0f);
            Application::get().getCameraController().move(delta, 0.1);
        }

        if (input.IsKeyPressed(KeyCode::E)) {
            glm::vec3 delta = glm::vec3(0.0f, 0.5, 0.0f);
            Application::get().getCameraController().move(delta, 0.1);
        }

        if (input.IsKeyPressed(KeyCode::Backspace) && m_GameObject.isActive()) {
            auto &coordinator = gameLayer().getCoordinator();
            if (!coordinator.HasComponent<EditToolComponent>(m_GameObject.entity())) {
                m_GameObject.destroy();
            }
        }

        if (input.IsKeyPressed(KeyCode::Space) && m_GameObject.isActive()) {
            glm::quat deltaRotation = glm::angleAxis(glm::radians(1.0f), camera.upVec());
            Application::get().getCameraController().spin(m_GameObject.position(),
                                                          deltaRotation * camera.rotationQuat(), 0.0);
        }

        if (input.IsKeyPressed(KeyCode::F) && m_GameObject.isActive()) {
            glm::vec3 curDir = camera.frontVec();
            glm::vec3 newDir = glm::normalize(m_GameObject.position() - camera.positionVec());

            glm::vec3 half = glm::normalize(curDir + newDir);
            glm::quat deltaRotation = glm::quat(glm::dot(curDir, half), glm::cross(curDir, half));

            Application::get().getCameraController().rotateTo(deltaRotation * camera.rotationQuat(), 0.2);
        }

        if (input.IsAnyKeyPressed({KeyCode::X, KeyCode::Y, KeyCode::Z})) {
            glm::vec3 cameraPos = camera.positionVec();
            glm::vec3 cameraDir = camera.frontVec();

            glm::vec3 planeNormal, newDir;

            if (input.IsKeyPressed(KeyCode::X)) {
                planeNormal = glm::vec3(-1.0f, 0.0f, 0.0f);
                newDir = glm::vec3(1.0, 0.0f, 0.001f);

                if (input.IsKeyPressed(KeyCode::Shift)) {
                    newDir = glm::vec3(-1.0, 0.0f, 0.001f);
                }
            }

            if (input.IsKeyPressed(KeyCode::Y)) {
                planeNormal = glm::vec3(0.0f, 1.0f, 0.0f);
                newDir = glm::vec3(0.001f, -1.0f, 0.0f);

                if (input.IsKeyPressed(KeyCode::Shift)) {
                    newDir = glm::vec3(0.001f, 1.0f, 0.0f);
                }
            }

            if (input.IsKeyPressed(KeyCode::Z)) {
                planeNormal = glm::vec3(0.0f, 0.0f, -1.0f);
                newDir = glm::vec3(0.001f, 0.0f, 1.0f);

                if (input.IsKeyPressed(KeyCode::Shift)) {
                    newDir = glm::vec3(0.001f, 0.0f, -1.0f);
                }
            }

            glm::vec3 planePoint = glm::vec3(0.0f, 0.0f, 0.0f);
            glm::vec3 planePointToCamera = cameraPos - planePoint;

            float normalDistance = glm::dot(planePointToCamera, planeNormal);
            float dot = glm::dot(glm::reflect(cameraDir, planeNormal), planeNormal);
            float t = normalDistance / dot;

            glm::vec3 pivot;
            if (std::abs(glm::dot(cameraDir, planeNormal)) < 0.01f) {
                pivot = m_Pivot;
            } else {
                pivot = cameraPos + cameraDir * t;
                m_Pivot = pivot;
            }

            glm::vec3 half = glm::normalize(cameraDir + newDir);
            glm::quat deltaRotation = glm::quat(glm::dot(cameraDir, half), glm::cross(cameraDir, half));

            glm::vec3 newPosition = pivot - newDir * std::abs(normalDistance);

            Application::get().getCameraController().spin(pivot, deltaRotation * camera.rotationQuat(), 0.4f);
        }
    }

    m_ScenePanel->onUpdate();
    m_SceneHierarchyPanel->onUpdate();
    m_GamePanel->onUpdate();
    m_ToolbarPanel->onUpdate();
    m_RenderPanel->onUpdate();
    m_InspectorPanel->onUpdate();
    m_ConsolePanel->onUpdate();
    m_ContentBrowserPanel->onUpdate();

    for (auto &tool : m_SceneTools) {
        if (tool.visible()) {
            tool.view->show();
            tool.view->onUpdate();
        } else {
            tool.view->hide();
        }
    }
}

void EditToolsLayer::onDraw() {
    const auto &window = Application::get().getWindow();

    int dWidth, dHeight;
    window.getDrawableSize(dWidth, dHeight);

    m_Imgui.Begin();

    m_ScenePanel->onDraw();

    onBeforeGameDraw();
    m_GamePanel->onDraw();
    onAfterGameDraw();

    m_RenderPanel->onDraw();
    m_ToolbarPanel->onDraw();
    m_SceneHierarchyPanel->onDraw();
    m_InspectorPanel->onDraw();
    m_ConsolePanel->onDraw();
    m_ContentBrowserPanel->onDraw();

    for (auto &tool : m_SceneTools) {
        if (tool.view->isVisible() && tool.visible()) {
            tool.view->onDraw();
        }
    }

    m_Imgui.End();
}

void EditToolsLayer::onBeforeGameDraw() {
    for (size_t i = 0; i < m_SceneTools.size(); i++) {
        m_SceneToolsVisibility[i] = m_SceneTools[i].view->isVisible();
        if (m_SceneToolsVisibility[i]) {
            m_SceneTools[i].view->hide();
        }
    }
    m_InspectorPanel->onBeforeGameDraw();
}

void EditToolsLayer::onAfterGameDraw() {
    for (size_t i = 0; i < m_SceneTools.size(); i++) {
        if (m_SceneToolsVisibility[i]) {
            m_SceneTools[i].view->show();
        }
    }
    m_InspectorPanel->onAfterGameDraw();
}

void EditToolsLayer::onDetach() {
    m_SceneHierarchyPanel->onDetach();
    m_ContentBrowserPanel->onDetach();
    m_RenderPanel->onDetach();
    m_ToolbarPanel->onDetach();
    m_ScenePanel->onDetach();
    m_GamePanel->onDetach();
    m_InspectorPanel->onDetach();
    m_Imgui.OnDetach();

    for (auto &tool : m_SceneTools) {
        tool.view->onDetach();
    }
}

void EditToolsLayer::onMouseEvent(MouseEvent &event) {
    m_ScenePanel->onMouseEvent(event);
    if (event.handled) {
        return;
    }

    if (ImGui::GetIO().WantCaptureMouse && m_ScenePanel->blockEvents()) {
        return;
    }

    for (auto &tool : m_SceneTools) {
        tool.view->onMouseEvent(event);
        if (event.handled) {
            return;
        }
    }

    m_InspectorPanel->onMouseEvent(event);
    if (event.handled) {
        return;
    }

    if (event.type == EventType::MouseDown) {
        handleSelection();
    }

    if (!event.handled && event.type == EventType::MouseWheel && Application::get().getTime().poused()) {
        auto mousePos = Application::get().getInput().GetMousePosition();
        auto &camera = Application::get().getCamera();
        auto cameraRotation = camera.rotationQuat();

        float eventXSign = event.x > 0.0f ? 1.0f : -1.0f;
        float eventYSign = event.y > 0.0f ? 1.0f : -1.0f;

        glm::vec2 mouseOffset =
            glm::vec2(-1.0f * eventXSign * std::pow(event.x, 2), eventYSign * std::pow(event.y, 2)) * 0.015f;

        auto deltaRotationX =
            glm::angleAxis(mouseOffset.y, glm::normalize(glm::cross(camera.frontVec(), camera.upVec())));
        auto deltaRotationY = glm::angleAxis(mouseOffset.x, camera.upVec());

        Application::get().getCameraController().rotateTo(deltaRotationX * deltaRotationY * cameraRotation, 0.1);
    }
    m_MousePos = Application::get().getInput().GetMousePosition();
}

void EditToolsLayer::handleSelection() {
    auto &gameCoordinator = gameLayer().getCoordinator();

    auto ray = Application::get().getMousePicker().ray();
    auto pos = Application::get().getCamera().positionVec();

    if (!m_Coordinator.HasEntity(m_ScenePanel->entity()) && !gameCoordinator.HasEntity(m_ScenePanel->entity())) {
        m_GameObject.setEntity(c_NoEntity);
        return;
    }

    if (m_Coordinator.HasEntity(m_ScenePanel->entity())) {
        Entity entity = m_ScenePanel->entity();

        for (auto &tool : m_SceneTools) {
            if (tool.visible() && tool.view->handleSelection(entity)) {
                return;
            }
        }

        if (m_InspectorPanel->handleSelection(entity)) {
            return;
        }
    }

    if (m_ScenePanel->entity() != c_NoEntity && gameCoordinator.HasEntity(m_ScenePanel->entity())) {
        if (gameCoordinator.HasComponent<EditToolComponent>(m_ScenePanel->entity())) {
            const auto &editTool = gameCoordinator.GetComponent<EditToolComponent>(m_ScenePanel->entity());
            if (!editTool.canSelect) {
                return;
            }
        }
        m_GameObject.setEntity(m_ScenePanel->entity());
    }
}

} // namespace Engine
