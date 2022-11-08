#include "TransformControlsPosition.hpp"

#include "Application.hpp"
#include "Configs.hpp"
#include "InstancedModel.hpp"
#include "LocationComponent.hpp"
#include "ModelLoader.hpp"
#include "Render3DComponent.hpp"
#include "StaticCollisionComponent.hpp"
#include "TagComponent.hpp"

namespace Engine {

TransformControlsPosition::TransformControlsPosition(GameObjectModel &model) : m_Model(model) {}

void TransformControlsPosition::onAttach() {
    auto &coordinator = toolsLayer().getCoordinator();
    auto& materials = Application::get().getMaterials();

    Application::get().getModels().RegisterModel(Configs::c_EditToolsModelPrefix + "arrow-x",
                                                 ModelLoader::load("./assets/models/box/arrow-x.fbx"));

    Application::get().getModels().RegisterModel(Configs::c_EditToolsModelPrefix + "arrow-y",
                                                 ModelLoader::load("./assets/models/box/arrow-y.fbx"));

    Application::get().getModels().RegisterModel(Configs::c_EditToolsModelPrefix + "arrow-z",
                                                 ModelLoader::load("./assets/models/box/arrow-z.fbx"));

    auto controlX = coordinator.CreateEntity("arrow-x");
    auto locationX = LocationComponent();
    locationX.rotation = glm::vec3(0.0f, 1.57f, 0.0f);
    coordinator.AddComponent(controlX, locationX);
    coordinator.AddComponent(controlX, Render3DComponent(Configs::c_EditToolsModelPrefix + "arrow-x", materials.sceneToolMaterial.get(), 0.1f));
    coordinator.AddComponent(controlX, StaticCollisionComponent(0.5, 0.5, 0.5));
    coordinator.AddComponent(controlX, TagComponent("arrow"));
    m_ControlX = controlX;

    auto controlY = coordinator.CreateEntity("arrow-y");
    auto locationY = LocationComponent();
    locationY.rotation = glm::vec3(-1.57f, 0.0f, 0.0f);
    coordinator.AddComponent(controlY, locationY);
    coordinator.AddComponent(controlY, Render3DComponent(Configs::c_EditToolsModelPrefix + "arrow-y", materials.sceneToolMaterial.get(), 0.1f));
    coordinator.AddComponent(controlY, StaticCollisionComponent(0.5, 0.5, 0.5));
    coordinator.AddComponent(controlY, TagComponent("arrow"));
    m_ControlY = controlY;

    auto controlZ = coordinator.CreateEntity("arrow-z");
    auto locationZ = LocationComponent();
    locationZ.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    coordinator.AddComponent(controlZ, locationZ);
    coordinator.AddComponent(controlZ, Render3DComponent(Configs::c_EditToolsModelPrefix + "arrow-z", materials.sceneToolMaterial.get(), 0.1f));
    coordinator.AddComponent(controlZ, StaticCollisionComponent(0.5, 0.5, 0.5));
    coordinator.AddComponent(controlZ, TagComponent("arrow"));
    m_ControlZ = controlZ;
}

void TransformControlsPosition::onUpdate() {
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

    if (m_Model.transformOrientation() == GameObjectModel::TransformOrientation::Global) {
        locationX.position = m_Model.position() + glm::vec3(2.0f, 0.0f, 0.0f);
        locationY.position = m_Model.position() + glm::vec3(0.0f, 2.0f, 0.0f);
        locationZ.position = m_Model.position() + glm::vec3(0.0f, 0.0f, 2.0f);

        locationX.rotation = glm::vec3(0.0f, 1.57f, 0.0f);
        locationY.rotation = glm::vec3(-1.57f, 0.0f, 0.0f);
        locationZ.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    } else {
        locationX.position = m_Model.position() + glm::quat(m_Model.rotation()) * glm::vec3(2.0f, 0.0f, 0.0f);
        locationY.position = m_Model.position() + glm::quat(m_Model.rotation()) * glm::vec3(0.0f, 2.0f, 0.0f);
        locationZ.position = m_Model.position() + glm::quat(m_Model.rotation()) * glm::vec3(0.0f, 0.0f, 2.0f);

        locationX.rotation = glm::eulerAngles(glm::quat(m_Model.rotation()) * glm::quat(glm::vec3(0.0f, 1.57f, 0.0f)));
        locationY.rotation = glm::eulerAngles(glm::quat(m_Model.rotation()) * glm::quat(glm::vec3(-1.57f, 0.0f, 0.0f)));
        locationZ.rotation = glm::eulerAngles(glm::quat(m_Model.rotation()) * glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)));
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

void TransformControlsPosition::onDetach() {}

void TransformControlsPosition::onMouseEvent(MouseEvent &event) {
    if (event.type == EventType::MouseMoved) {
        if (m_ActiveControl == m_ControlX || m_ActiveControl == m_ControlY || m_ActiveControl == m_ControlZ) {
            onTransform();
            event.handled = true;
        }
    }

    if (event.type == EventType::MouseUp) {
        m_ActiveControl = c_NoEntity;
    }

    m_PrevMouseRay = Application::get().getMousePicker().ray();

    m_HoveredControl = *(static_cast<Entity *>(event.data));
}

bool TransformControlsPosition::handleSelection(Entity entity) {
    m_ActiveControl = entity;
    float selected = entity == m_ControlX || entity == m_ControlY || entity == m_ControlZ;

    if (selected) {
        glm::vec3 axis = glm::vec3(0.0f);

        if (m_ActiveControl == m_ControlX) {
            axis = glm::vec3(1.0f, 0.0f, 0.0f);
        }
        if (m_ActiveControl == m_ControlY) {
            axis = glm::vec3(0.0f, 1.0f, 0.0f);
        }
        if (m_ActiveControl == m_ControlZ) {
            axis = glm::vec3(0.0f, 0.0f, 1.0f);
        }

        auto cameraPos = Application::get().getCamera().positionVec();
        auto mouseRay = Application::get().getMousePicker().ray();
        auto objPos = m_Model.position();
        auto toObjRay = glm::normalize(objPos - cameraPos);

        m_GrabOffset = Math::moveAlongAxis(cameraPos, m_Model.position(), toObjRay, mouseRay, axis);
    }

    return selected;
}

void TransformControlsPosition::hide() {
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

void TransformControlsPosition::onTransform() {
    glm::vec3 axis = glm::vec3(0.0f);

    if (m_ActiveControl == m_ControlX) {
        axis = glm::vec3(1.0f, 0.0f, 0.0f);
    }
    if (m_ActiveControl == m_ControlY) {
        axis = glm::vec3(0.0f, 1.0f, 0.0f);
    }
    if (m_ActiveControl == m_ControlZ) {
        axis = glm::vec3(0.0f, 0.0f, 1.0f);
    }

    auto cameraPos = Application::get().getCamera().positionVec();
    auto mouseRay = Application::get().getMousePicker().ray();
    auto objPos = m_Model.position();
    auto toObjRay = glm::normalize(objPos - cameraPos);

    glm::vec3 move = Math::moveAlongAxis(cameraPos, m_Model.position() + m_GrabOffset, m_PrevMouseRay, mouseRay, axis);

    if (m_Model.transformOrientation() == GameObjectModel::TransformOrientation::Local) {
        move = glm::quat(m_Model.rotation()) * move;
        m_Model.moveLocal(move);
    } else {
        m_Model.move(move);
    }
}

} // namespace Engine
