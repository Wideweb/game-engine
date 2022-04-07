#include "EditToolsLayer.hpp"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp>
#include <limits>

#include "Application.hpp"
#include "EditToolComponent.hpp"
#include "Input.hpp"
#include "Mesh2D.hpp"
#include "TextureLoader.hpp"

namespace Engine {

Layer &EditToolsLayer::gameLayer() { return Application::get().getLayer("game_area"); }

void EditToolsLayer::onAttach() {
    Application::get().getTime().stop();
    Application::get().getCamera().setPosition(glm::vec3(8.0f, 6.0f, 8.0f));
    Application::get().getCamera().setRotation(glm::quat(glm::vec3(glm::radians(-25.0f), glm::radians(45.0f), 0.0f)));

    Application::get().getTextures().add("engine_placeholder", TextureLoader::placeholder());

    auto &eventHandler = Application::get().getEventHandler();
    eventHandler.add<ApplicationError>([this](const ApplicationError &e) {
        m_Logs += e.what();
        m_Logs += "\n";
    });

    m_GameObject.entityChange$.addEventCallback([this](Entity entity) {
        if (Application::get().getTime().poused()) {
            ImGui::SetWindowFocus();
        }
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

    m_TerrainPanel = std::make_unique<TerrainPanel>(m_GameObject);
    m_TerrainPanel->onAttach();
    m_TerrainPanel->hide();

    m_CollisionPanel = std::make_unique<CollisionPanel>(m_GameObject);
    m_CollisionPanel->onAttach();
    m_CollisionPanel->hide();

    m_RigitBodyPanel = std::make_unique<RigitBodyPanel>(m_GameObject);
    m_RigitBodyPanel->onAttach();
    m_RigitBodyPanel->hide();

    m_VelocityPanel = std::make_unique<VelocityPanel>(m_GameObject);
    m_VelocityPanel->onAttach();
    m_VelocityPanel->hide();

    m_ParticlesPanel = std::make_unique<ParticlesPanel>(m_GameObject);
    m_ParticlesPanel->onAttach();

    m_DirectedLightPanel = std::make_unique<DirectedLightPanel>(m_GameObject);
    m_DirectedLightPanel->onAttach();
    m_DirectedLightPanel->hide();

    m_ModelTransformPanel = std::make_unique<ModelTransformPanel>(m_GameObject);
    m_ModelTransformPanel->onAttach();
    m_ModelTransformPanel->hide();

    m_MaterialPanel = std::make_unique<MaterialPanel>(m_GameObject);
    m_MaterialPanel->onAttach();
    m_MaterialPanel->hide();

    m_BehaviourPanel = std::make_unique<BehaviourPanel>(m_GameObject);
    m_BehaviourPanel->onAttach();
    m_BehaviourPanel->hide();

    m_SkeletPanel = std::make_unique<SkeletPanel>(m_GameObject);
    m_SkeletPanel->onAttach();
    m_SkeletPanel->hide();

    m_TerrainTransform = std::make_unique<TerrainTransform>(m_GameObject, *m_TerrainPanel);
    m_TerrainTransform->onAttach();
    m_TerrainTransform->hide();

    m_MeshBody = std::make_unique<MeshBody>(m_GameObject);
    m_MeshBody->onAttach();
    m_MeshBody->hide();

    m_TransformControlsPosition = std::make_unique<TransformControlsPosition>(m_GameObject);
    m_TransformControlsPosition->onAttach();
    m_TransformControlsPosition->hide();

    m_TransformControlsRotation = std::make_unique<TransformControlsRotation>(m_GameObject);
    m_TransformControlsRotation->onAttach();
    m_TransformControlsRotation->hide();

    m_TransformControlsScale = std::make_unique<TransformControlsScale>(m_GameObject);
    m_TransformControlsScale->onAttach();
    m_TransformControlsScale->hide();

    m_CollisionBody = std::make_unique<CollisionBody>(m_GameObject);
    m_CollisionBody->onAttach();
    m_CollisionBody->hide();

    m_SkyboxPanel = std::make_unique<SkyboxPanel>();
    m_SkyboxPanel->onAttach();
    m_SkyboxPanel->show();

    m_GamePanel = std::make_unique<GamePanel>();
    m_GamePanel->onAttach();
    m_GamePanel->show();

    m_CameraDirector = std::make_unique<CameraDirector>(m_GameObject);
    m_CameraDirector->onAttach();

    m_DirectedLightDirector = std::make_unique<DirectedLightDirector>(m_GameObject);
    m_DirectedLightDirector->onAttach();
}

void EditToolsLayer::onUpdate() {
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
                coordinator.DestroyEntity(m_GameObject.entity());
                m_GameObject.setEntity(c_NoEntity);
            }
        }

        if (input.IsKeyPressed(KeyCode::Space) && m_GameObject.isActive()) {
            // glm::vec3 distance = camera.positionVec() - m_GameObject.position();
            glm::quat deltaRotation = glm::angleAxis(glm::radians(1.0f), camera.upVec());
            // glm::vec3 newPosition = m_GameObject.position() + deltaRotation * distance;
            // camera.setPosition(newPosition);
            // camera.setRotation(deltaRotation * camera.rotationQuat());
            Application::get().getCameraController().spin(m_GameObject.position(),
                                                          deltaRotation * camera.rotationQuat(), 0.0);
        }

        if (input.IsKeyPressed(KeyCode::F) && m_GameObject.isActive()) {
            glm::vec3 curDir = camera.frontVec();
            glm::vec3 newDir = glm::normalize(m_GameObject.position() - camera.positionVec());

            glm::vec3 half = glm::normalize(curDir + newDir);
            glm::quat deltaRotation = glm::quat(glm::dot(curDir, half), glm::cross(curDir, half));

            // camera.setRotation(deltaRotation * camera.rotationQuat());

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

    m_Imgui.onUpdate();

    if (m_GamePanel->isVisible()) {
        m_GamePanel->onUpdate();
    }

    if (m_RenderPanel->isVisible()) {
        m_RenderPanel->onUpdate();
    }

    if (!Application::get().getTime().poused()) {
        m_GameObject.setEntity(c_NoEntity);
    }

    if (m_GameObject.isActive()) {
        m_TransformPanel->show();
        m_MeshBody->show();
    } else {
        m_TransformPanel->hide();
        m_MeshBody->hide();
    }

    if (m_GameObject.isActive() && m_GameObject.hasPosition()) {
        m_TransformControlsPosition->show();
    } else {
        m_TransformControlsPosition->hide();
    }

    if (m_GameObject.isActive() && m_GameObject.hasTerrainCollision()) {
        m_TerrainTransform->show();
    } else {
        m_TerrainTransform->hide();
    }

    if (m_GameObject.isActive() && m_GameObject.hasTerrainCollision()) {
        m_TerrainPanel->show();
    } else {
        m_TerrainPanel->hide();
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

    if (m_GameObject.isActive() && (m_GameObject.hasCollision())) {
        m_CollisionBody->show();
    } else {
        m_CollisionBody->hide();
    }

    if (m_GameObject.isActive() && m_GameObject.hasCollision()) {
        m_CollisionPanel->show();
    } else {
        m_CollisionPanel->hide();
    }

    if (m_GameObject.isActive() && m_GameObject.hasPhysics()) {
        m_RigitBodyPanel->show();
    } else {
        m_RigitBodyPanel->hide();
    }

    if (m_GameObject.isActive() && m_GameObject.hasVelocity()) {
        m_VelocityPanel->show();
    } else {
        m_VelocityPanel->hide();
    }

    if (m_GameObject.isActive() && m_GameObject.hasMaterial()) {
        m_ModelTransformPanel->show();
    } else {
        m_ModelTransformPanel->hide();
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

    if (m_GameObject.isActive() && m_GameObject.hasBehaviour()) {
        m_BehaviourPanel->show();
    } else {
        m_BehaviourPanel->hide();
    }

    if (m_TransformControlsPosition->isVisible()) {
        m_TransformControlsPosition->onUpdate();
    }

    if (m_TerrainTransform->isVisible()) {
        m_TerrainTransform->onUpdate();
    }

    if (m_TransformControlsRotation->isVisible()) {
        m_TransformControlsRotation->onUpdate();
    }

    if (m_TransformControlsScale->isVisible()) {
        m_TransformControlsScale->onUpdate();
    }

    if (m_CollisionBody->isVisible()) {
        m_CollisionBody->onUpdate();
    }

    if (m_CollisionPanel->isVisible()) {
        m_CollisionPanel->onUpdate();
    }

    if (m_RigitBodyPanel->isVisible()) {
        m_RigitBodyPanel->onUpdate();
    }

    if (m_TransformPanel->isVisible()) {
        m_TransformPanel->onUpdate();
    }

    if (m_TerrainPanel->isVisible()) {
        m_TerrainPanel->onUpdate();
    }

    if (m_VelocityPanel->isVisible()) {
        m_VelocityPanel->onUpdate();
    }

    if (m_ModelTransformPanel->isVisible()) {
        m_ModelTransformPanel->onUpdate();
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

    if (m_SkyboxPanel->isVisible()) {
        m_SkyboxPanel->onUpdate();
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
}

void EditToolsLayer::onDraw() {
    const auto &window = Application::get().getWindow();

    int dWidth, dHeight;
    window.getDrawableSize(dWidth, dHeight);

    m_Imgui.Begin();

    ImGui::Begin("Console");
    const char *logs = m_Logs.data();
    ImGui::Text(logs);
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);
    ImGui::End();

    if (m_GamePanel->isVisible()) {
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

    if (m_TerrainTransform->isVisible()) {
        m_TerrainTransform->onDraw(0, 0);
    }

    if (m_TransformControlsRotation->isVisible()) {
        m_TransformControlsRotation->onDraw(0, 0);
    }

    if (m_TransformControlsScale->isVisible()) {
        m_TransformControlsScale->onDraw(0, 0);
    }

    if (m_CollisionBody->isVisible()) {
        m_CollisionBody->onDraw(0, 0);
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::PopStyleVar();

    if (m_TransformPanel->isVisible()) {
        m_TransformPanel->onDraw(0, 250);
    }

    if (m_VelocityPanel->isVisible()) {
        m_VelocityPanel->onDraw(0, 0);
    }

    if (m_ModelTransformPanel->isVisible()) {
        m_ModelTransformPanel->onDraw(0, 250);
    }

    if (m_MaterialPanel->isVisible()) {
        m_MaterialPanel->onDraw(0, 250);
    }

    if (m_CollisionPanel->isVisible()) {
        m_CollisionPanel->onDraw(0, 0);
    }

    if (m_RigitBodyPanel->isVisible()) {
        m_RigitBodyPanel->onDraw(0, 0);
    }

    if (m_TerrainPanel->isVisible()) {
        m_TerrainPanel->onDraw(0, 250);
    }

    if (m_BehaviourPanel->isVisible()) {
        m_BehaviourPanel->onDraw(0, 250);
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

    ImGui::End();

    if (m_MeshBody->isVisible()) {
        m_MeshBody->onDraw(0, 250);
    }

    if (m_SkyboxPanel->isVisible()) {
        m_SkyboxPanel->onDraw(0, 0);
    }

    m_Imgui.End();
}

void EditToolsLayer::onDetach() {
    m_GamePanel->onDetach();
    m_CameraDirector->onDetach();
    m_DirectedLightDirector->onDetach();
    m_TransformControlsPosition->onDetach();
    m_TerrainTransform->onDetach();
    m_TransformControlsRotation->onDetach();
    m_TransformControlsScale->onDetach();
    m_CollisionBody->onDetach();
    m_CollisionPanel->onDetach();
    m_RigitBodyPanel->onDetach();
    m_TransformPanel->onDetach();
    m_TerrainPanel->onDetach();
    m_VelocityPanel->onDetach();
    m_ParticlesPanel->onDetach();
    m_DirectedLightPanel->onDetach();
    m_SkyboxPanel->onDetach();
    m_ModelTransformPanel->onDetach();
    m_MaterialPanel->onDetach();
    m_BehaviourPanel->onDetach();
    m_MeshBody->onDetach();
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

    if (m_TerrainTransform->isVisible()) {
        m_TerrainTransform->onMouseEvent(event);
        if (event.handled) {
            return;
        }
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

        // glm::vec2 mouseOffset = (mousePos - m_MousePos) * 0.002f;
        glm::vec2 mouseOffset =
            glm::vec2(-1.0f * eventXSign * std::pow(event.x, 2), eventYSign * std::pow(event.y, 2)) * 0.015f;
        // float speed = mouseOffset.x * mouseOffset.x + mouseOffset.y * mouseOffset.y;

        // std::cout << speed << std::endl;

        auto deltaRotationX =
            glm::angleAxis(mouseOffset.y, glm::normalize(glm::cross(camera.frontVec(), camera.upVec())));
        auto deltaRotationY = glm::angleAxis(mouseOffset.x, camera.upVec());

        // camera.setRotation(deltaRotationX * deltaRotationY * cameraRotation);

        Application::get().getCameraController().rotateTo(deltaRotationX * deltaRotationY * cameraRotation, 0.1);
    }
    m_MousePos = Application::get().getInput().GetMousePosition();
}

void EditToolsLayer::handleSelection() {
    auto &gameCoordinator = gameLayer().getCoordinator();

    auto ray = Application::get().getMousePicker().ray();
    auto pos = Application::get().getCamera().positionVec();
    // auto editToolsEntities = m_Collision.Raycast(pos, ray, 100.0f);

    if (!m_Coordinator.HasEntity(m_Imgui.entity()) && !gameCoordinator.HasEntity(m_Imgui.entity())) {
        m_GameObject.setEntity(c_NoEntity);
        return;
    }

    if (m_Coordinator.HasEntity(m_Imgui.entity())) {
        Entity entity = m_Imgui.entity();

        if (m_CameraDirector->isVisible() && m_CameraDirector->handleSelection(entity)) {
            return;
        }

        if (m_DirectedLightDirector->isVisible() && m_DirectedLightDirector->handleSelection(entity)) {
            return;
        }

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

    if (m_Imgui.entity() != c_NoEntity && gameCoordinator.HasEntity(m_Imgui.entity())) {
        if (gameCoordinator.HasComponent<EditToolComponent>(m_Imgui.entity())) {
            const auto &editTool = gameCoordinator.GetComponent<EditToolComponent>(m_Imgui.entity());
            if (!editTool.canSelect) {
                return;
            }
        }
        m_GameObject.setEntity(m_Imgui.entity());
    }
}

} // namespace Engine
