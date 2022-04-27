#include "InspectorPanel.hpp"

#include "ImGuiWidgets.hpp"

#include "BehaviourPanel.hpp"
#include "CollisionPanel.hpp"
#include "DirectedLightPanel.hpp"
#include "ModelRenderPanel.hpp"
#include "ParticlesPanel.hpp"
#include "RigitBodyPanel.hpp"
#include "SkeletPanel.hpp"
#include "TerrainPanel.hpp"
#include "Text2DPanel.hpp"
#include "Transform2DPanel.hpp"
#include "TransformPanel.hpp"
#include "VelocityPanel.hpp"

#include "CollisionBody.hpp"
#include "MeshBody.hpp"
#include "TerrainTransform.hpp"
#include "TransformControlsPosition.hpp"
#include "TransformControlsRotation.hpp"
#include "TransformControlsScale.hpp"

#include "BehaviourComponent.hpp"
#include "CollisionComponent.hpp"
#include "DirectedLightComponent.hpp"
#include "Location2DComponent.hpp"
#include "LocationComponent.hpp"
#include "ParticlesComponent.hpp"
#include "PhysicsComponent.hpp"
#include "Render3DComponent.hpp"
#include "SkeletComponent.hpp"
#include "StaticCollisionComponent.hpp"
#include "TerrainCollisionComponent.hpp"
#include "Text2DComponent.hpp"
#include "VelocityComponent.hpp"

#include "imgui/imgui.h"
#include <functional>

namespace Engine {

InspectorPanel::InspectorPanel(GameObjectModel &model) : m_GameObject(model) {}

void InspectorPanel::onAttach() {
    auto transformPanel = std::make_shared<TransformPanel>(m_GameObject);
    auto transformPositionTool = std::make_shared<TransformControlsPosition>(m_GameObject);
    auto TransformRotationTool = std::make_shared<TransformControlsRotation>(m_GameObject);
    auto TransformScaleTool = std::make_shared<TransformControlsScale>(m_GameObject);
    auto transform2DPanel = std::make_shared<Transform2DPanel>(m_GameObject);
    auto collisionPanel = std::make_shared<CollisionPanel>(m_GameObject);
    auto rigitBodyPanel = std::make_shared<RigitBodyPanel>(m_GameObject);
    auto velocityPanel = std::make_shared<VelocityPanel>(m_GameObject);
    auto particlesPanel = std::make_shared<ParticlesPanel>(m_GameObject);
    auto directedLightPanel = std::make_shared<DirectedLightPanel>(m_GameObject);
    auto modelRenderPanel = std::make_shared<ModelRenderPanel>(m_GameObject);
    auto behaviourPanel = std::make_shared<BehaviourPanel>(m_GameObject);
    auto skeletPanel = std::make_shared<SkeletPanel>(m_GameObject);
    auto text2DPanel = std::make_shared<Text2DPanel>(m_GameObject);
    auto terrainPanel = std::make_shared<TerrainPanel>(m_GameObject);
    auto terrainTransformTool = std::make_shared<TerrainTransform>(m_GameObject, *terrainPanel);
    auto collisionBodyTool = std::make_shared<CollisionBody>(m_GameObject);
    auto meshBodyTool = std::make_shared<MeshBody>(m_GameObject);

    addPanel<LocationComponent>(transformPanel);
    addPanel<Location2DComponent>(transform2DPanel);
    addPanel<CollisionComponent, StaticCollisionComponent>(collisionPanel);
    addPanel<PhysicsComponent>(rigitBodyPanel);
    addPanel<VelocityComponent>(velocityPanel);
    addPanel<ParticlesComponent>(particlesPanel);
    addPanel<DirectedLightComponent>(directedLightPanel);
    addPanel<Render3DComponent>(modelRenderPanel);
    addPanel<BehaviourComponent>(behaviourPanel);
    addPanel<SkeletComponent>(skeletPanel);
    addPanel<Text2DComponent>(text2DPanel);
    addPanel<TerrainCollisionComponent>(terrainPanel);

    addSceneTool<LocationComponent>(transformPositionTool);
    addSceneTool<LocationComponent>(TransformRotationTool);
    addSceneTool<LocationComponent>(TransformScaleTool);
    addSceneTool<TerrainCollisionComponent>(terrainTransformTool);
    addSceneTool<CollisionComponent, StaticCollisionComponent>(collisionBodyTool);
    addSceneTool<Render3DComponent>(meshBodyTool);

    for (auto &tool : m_Tools) {
        tool->view->onAttach();
        tool->view->hide();
    }
}

void InspectorPanel::onUpdate() {
    for (auto &tool : m_Tools) {
        if (tool->visible()) {
            tool->view->show();
            tool->view->onUpdate();
        } else {
            tool->view->hide();
        }
    }
}

void InspectorPanel::onDraw() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::PopStyleVar();

    if (m_GameObject.isActive()) {
        static char entityName[64];
        strcpy(entityName, m_GameObject.entityName().c_str());
        ImGuiWidgets::PaddingTop(10.0f);
        ImGuiWidgets::PaddingLeft(10.0f);
        ImGui::InputText("##entityName", entityName, 64);
        ImGuiWidgets::PaddingTop(5.0f);
        std::string newEntityName(entityName);
        if (newEntityName != m_GameObject.entityName()) {
            m_GameObject.entityName(newEntityName);
        }
    }

    for (auto &tool : m_Tools) {
        if (tool->view->isVisible() && tool->visible()) {
            tool->view->onDraw();
        }
    }

    ImGui::NewLine();
    ImGui::End();
}

void InspectorPanel::onBeforeGameDraw() {
    size_t index = 0;
    for (auto &tool : m_Tools) {
        if (tool->type == IComponentEditTool::Type::SceneTool) {
            m_SceneToolsVisibility[index] = tool->view->isVisible();
            if (m_SceneToolsVisibility[index]) {
                tool->view->hide();
            }
            ++index;
        }
    }
}

void InspectorPanel::onAfterGameDraw() {
    size_t index = 0;
    for (auto &tool : m_Tools) {
        if (tool->type == IComponentEditTool::Type::SceneTool) {
            if (m_SceneToolsVisibility[index++]) {
                tool->view->show();
            }
        }
    }
}

void InspectorPanel::onDetach() {
    for (auto &tool : m_Tools) {
        tool->view->onDetach();
    }
}

void InspectorPanel::onMouseEvent(MouseEvent &event) {
    for (auto &tool : m_Tools) {
        if (tool->view->isVisible() && tool->visible()) {
            tool->view->onMouseEvent(event);
            if (event.handled) {
                return;
            }
        }
    }
}

bool InspectorPanel::handleSelection(Entity entity) {
    for (auto &tool : m_Tools) {
        if (tool->view->isVisible() && tool->visible() && tool->view->handleSelection(entity)) {
            return true;
        }
    }

    return false;
}

} // namespace Engine
