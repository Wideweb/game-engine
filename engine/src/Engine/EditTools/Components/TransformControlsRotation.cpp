#include "TransformControlsRotation.hpp"

#include "Application.hpp"
#include "InstancedModel.hpp"
#include "LocationComponent.hpp"
#include "ModelLoader.hpp"
#include "Render3DComponent.hpp"
#include "StaticCollisionComponent.hpp"
#include "TagComponent.hpp"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

namespace Engine {

TransformControlsRotation::TransformControlsRotation(GameObjectModel &model) : m_Model(model) {}

void TransformControlsRotation::onAttach() {
    auto &coordinator = toolsLayer().getCoordinator();

    Application::get().getModels().RegisterModel("arc-x", ModelLoader::load("./assets/models/box/arc.obj"));
    {
        auto arcXModel = Application::get().getModels().GetModel<InstancedModel>("arc-x");
        auto &vertices = arcXModel->meshes[0].vertices;
        for (auto &vertex : vertices) {
            vertex.color = glm::vec3(1.0f, 0.0f, 0.0f);
        }
        arcXModel->update();
    }

    Application::get().getModels().RegisterModel("arc-y", ModelLoader::load("./assets/models/box/arc.obj"));
    {
        auto arcYModel = Application::get().getModels().GetModel<InstancedModel>("arc-y");
        auto &vertices = arcYModel->meshes[0].vertices;
        for (auto &vertex : vertices) {
            vertex.color = glm::vec3(0.0f, 1.0f, 0.0f);
        }
        arcYModel->update();
    }

    Application::get().getModels().RegisterModel("arc-z", ModelLoader::load("./assets/models/box/arc.obj"));
    {
        auto arcZModel = Application::get().getModels().GetModel<InstancedModel>("arc-z");
        auto &vertices = arcZModel->meshes[0].vertices;
        for (auto &vertex : vertices) {
            vertex.color = glm::vec3(0.0f, 0.0f, 1.0f);
        }
        arcZModel->update();
    }

    auto controlX = coordinator.CreateEntity("arc-x");
    auto renderX = Render3DComponent("arc-x", 0.1f, true);
    renderX.rotation = glm::vec3(1.57f, 1.57f, 0.0f);
    coordinator.AddComponent(controlX, renderX);
    coordinator.AddComponent(controlX, LocationComponent());
    coordinator.AddComponent(controlX, StaticCollisionComponent(0.25, 0.25, 0.25));
    coordinator.AddComponent(controlX, TagComponent("arc"));
    m_ControlX = controlX;

    auto controlY = coordinator.CreateEntity("arc-y");
    auto renderY = Render3DComponent("arc-y", 0.1f, true);
    renderY.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    coordinator.AddComponent(controlY, renderY);
    coordinator.AddComponent(controlY, LocationComponent());
    coordinator.AddComponent(controlY, StaticCollisionComponent(0.25, 0.25, 0.25));
    coordinator.AddComponent(controlY, TagComponent("arc"));
    m_ControlY = controlY;

    auto controlZ = coordinator.CreateEntity("arc-z");
    auto renderZ = Render3DComponent("arc-z", 0.1f, true);
    renderZ.rotation = glm::vec3(-1.57f, 0.0f, 0.0f);
    coordinator.AddComponent(controlZ, renderZ);
    coordinator.AddComponent(controlZ, LocationComponent());
    coordinator.AddComponent(controlZ, StaticCollisionComponent(0.25, 0.25, 0.25));
    coordinator.AddComponent(controlZ, TagComponent("arc"));
    m_ControlZ = controlZ;
}

void TransformControlsRotation::onUpdate() {}

void TransformControlsRotation::onDraw(int, int) {
    auto &gameCordinator = gameLayer().getCoordinator();
    auto &coordinator = toolsLayer().getCoordinator();

    auto &renderX = coordinator.GetComponent<Render3DComponent>(m_ControlX);
    auto &renderY = coordinator.GetComponent<Render3DComponent>(m_ControlY);
    auto &renderZ = coordinator.GetComponent<Render3DComponent>(m_ControlZ);

    if (m_ActiveControl == m_ControlX || (m_ActiveControl == c_NoEntity && m_HoveredControl == m_ControlX)) {
        renderX.scale = glm::vec3(0.12f);
    } else {
        renderX.scale = glm::vec3(0.1f);
    }

    if (m_ActiveControl == m_ControlY || (m_ActiveControl == c_NoEntity && m_HoveredControl == m_ControlY)) {
        renderY.scale = glm::vec3(0.12f);
    } else {
        renderY.scale = glm::vec3(0.1f);
    }

    if (m_ActiveControl == m_ControlZ || (m_ActiveControl == c_NoEntity && m_HoveredControl == m_ControlZ)) {
        renderZ.scale = glm::vec3(0.12f);
    } else {
        renderZ.scale = glm::vec3(0.1f);
    }

    renderX.updated = true;
    renderY.updated = true;
    renderZ.updated = true;

    auto &locationX = coordinator.GetComponent<LocationComponent>(m_ControlX);
    auto &locationY = coordinator.GetComponent<LocationComponent>(m_ControlY);
    auto &locationZ = coordinator.GetComponent<LocationComponent>(m_ControlZ);

    locationX.position = m_Model.position() + glm::vec3(1.5f, 0.5f, 0.0f);
    locationY.position = m_Model.position() + glm::vec3(0.0f, 1.5f, -0.5f);
    locationZ.position = m_Model.position() + glm::vec3(0.0f, -0.5f, 1.5f);

    if (m_Model.transformOrientation() == GameObjectModel::TransformOrientation::Global) {
        locationX.position = m_Model.position() + glm::vec3(1.5f, 0.5f, 0.0f);
        locationY.position = m_Model.position() + glm::vec3(0.0f, 1.5f, -0.5f);
        locationZ.position = m_Model.position() + glm::vec3(0.0f, -0.5f, 1.5f);

        locationX.rotation = glm::vec3(0.0f);
        locationY.rotation = glm::vec3(0.0f);
        locationZ.rotation = glm::vec3(0.0f);
    } else {
        locationX.position = m_Model.position() + glm::quat(m_Model.rotation()) * glm::vec3(1.5f, 0.5f, 0.0f);
        locationY.position = m_Model.position() + glm::quat(m_Model.rotation()) * glm::vec3(0.0f, 1.5f, -0.5f);
        locationZ.position = m_Model.position() + glm::quat(m_Model.rotation()) * glm::vec3(0.0f, -0.5f, 1.5f);

        locationX.rotation = m_Model.rotation();
        locationY.rotation = m_Model.rotation();
        locationZ.rotation = m_Model.rotation();
    }

    locationX.updated = true;
    locationY.updated = true;
    locationZ.updated = true;

    auto &collisionX = coordinator.GetComponent<StaticCollisionComponent>(m_ControlX);
    auto &collisionY = coordinator.GetComponent<StaticCollisionComponent>(m_ControlY);
    auto &collisionZ = coordinator.GetComponent<StaticCollisionComponent>(m_ControlZ);

    collisionX.updated = true;
    collisionY.updated = true;
    collisionZ.updated = true;
}

void TransformControlsRotation::onDetach() {}

void TransformControlsRotation::onMouseEvent(MouseEvent &event) {
    if (event.type == EventType::MouseMoved) {
        if (m_ActiveControl == m_ControlX || m_ActiveControl == m_ControlY || m_ActiveControl == m_ControlZ) {
            onTransform();
            event.handled = true;
        }
    }

    if (event.type == EventType::MouseUp) {
        m_ActiveControl = c_NoEntity;
    }

    m_PrevMouseWorldPos = Application::get().getMousePicker().ray();

    m_HoveredControl = *(static_cast<Entity *>(event.data));
}

bool TransformControlsRotation::handleSelection(Entity entity) {
    m_ActiveControl = entity;
    return entity == m_ControlX || entity == m_ControlY || entity == m_ControlZ;
}

void TransformControlsRotation::hide() {
    auto &coordinator = toolsLayer().getCoordinator();

    auto &renderX = coordinator.GetComponent<Render3DComponent>(m_ControlX);
    auto &renderY = coordinator.GetComponent<Render3DComponent>(m_ControlY);
    auto &renderZ = coordinator.GetComponent<Render3DComponent>(m_ControlZ);

    renderX.scale = glm::vec3(0.0f);
    renderY.scale = glm::vec3(0.0f);
    renderZ.scale = glm::vec3(0.0f);

    renderX.updated = true;
    renderY.updated = true;
    renderZ.updated = true;

    BaseView::hide();
}

void TransformControlsRotation::onTransform() {
    auto mousePos = Application::get().getMousePicker().ray();
    auto pos = Application::get().getCamera().positionVec();

    glm::vec3 dRotation = (mousePos - m_PrevMouseWorldPos) * glm::distance(pos, m_Model.position());

    if (m_ActiveControl == m_ControlX) {
        dRotation.x = -dRotation.y + dRotation.z;
        dRotation.y = 0;
        dRotation.z = 0;
    } else if (m_ActiveControl == m_ControlY) {
        dRotation.y = -(dRotation.x + dRotation.z);
        dRotation.x = 0;
        dRotation.z = 0;
    } else if (m_ActiveControl == m_ControlZ) {
        dRotation.z = dRotation.x + -dRotation.y;
        dRotation.x = 0;
        dRotation.y = 0;
    } else {
        dRotation = glm::vec3(0.0f);
    }

    if (m_Model.transformOrientation() == GameObjectModel::TransformOrientation::Local) {
        dRotation = glm::eulerAngles(glm::quat(m_Model.localRotation()) * glm::quat(dRotation));
    } else {
        dRotation = glm::eulerAngles(glm::quat(dRotation) * glm::quat(m_Model.localRotation()));
    }

    m_Model.localRotation(dRotation);
}

} // namespace Engine
