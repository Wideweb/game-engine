#include "TransformControlsScale.hpp"

#include "Application.hpp"
#include "Configs.hpp"
#include "InstancedModel.hpp"
#include "LocationComponent.hpp"
#include "Math.hpp"
#include "ModelLoader.hpp"
#include "Render3DComponent.hpp"
#include "StaticCollisionComponent.hpp"
#include "TagComponent.hpp"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

namespace Engine {

TransformControlsScale::TransformControlsScale(GameObjectModel &model) : m_Model(model) {}

void TransformControlsScale::onAttach() {
    auto &coordinator = toolsLayer().getCoordinator();

    {
        Application::get().getModels().RegisterModel(Configs::c_EditToolsModelPrefix + "scale-x",
                                                     ModelLoader::load("./assets/models/box/scale.obj"));

        auto model =
            Application::get().getModels().GetModel<InstancedModel>(Configs::c_EditToolsModelPrefix + "scale-x");
        auto &vertices = model->meshes[0].vertices;
        for (auto &vertex : vertices) {
            vertex.color = glm::vec3(1.0f, 0.0f, 0.0f);
        }
        model->update();
    }

    {
        Application::get().getModels().RegisterModel(Configs::c_EditToolsModelPrefix + "scale-y",
                                                     ModelLoader::load("./assets/models/box/scale.obj"));

        auto model =
            Application::get().getModels().GetModel<InstancedModel>(Configs::c_EditToolsModelPrefix + "scale-y");
        auto &vertices = model->meshes[0].vertices;
        for (auto &vertex : vertices) {
            vertex.color = glm::vec3(0.0f, 1.0f, 0.0f);
        }
        model->update();
    }

    {
        Application::get().getModels().RegisterModel(Configs::c_EditToolsModelPrefix + "scale-z",
                                                     ModelLoader::load("./assets/models/box/scale.obj"));

        auto model =
            Application::get().getModels().GetModel<InstancedModel>(Configs::c_EditToolsModelPrefix + "scale-z");
        auto &vertices = model->meshes[0].vertices;
        for (auto &vertex : vertices) {
            vertex.color = glm::vec3(0.0f, 0.0f, 1.0f);
        }
        model->update();
    }

    auto controlX = coordinator.CreateEntity("modelAxis-x");
    auto renderX = Render3DComponent(Configs::c_EditToolsModelPrefix + "scale-x", 0.1f, true);
    renderX.rotation = glm::vec3(0.0f, 0.0f, -1.57f);
    coordinator.AddComponent(controlX, renderX);
    coordinator.AddComponent(controlX, LocationComponent());
    coordinator.AddComponent(controlX, StaticCollisionComponent(0.5, 0.5, 0.5));
    coordinator.AddComponent(controlX, TagComponent("modelAxis"));
    m_ControlX = controlX;

    auto controlY = coordinator.CreateEntity("modelAxis-y");
    auto renderY = Render3DComponent(Configs::c_EditToolsModelPrefix + "scale-y", 0.1f, true);
    renderY.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    coordinator.AddComponent(controlY, renderY);
    coordinator.AddComponent(controlY, LocationComponent());
    coordinator.AddComponent(controlY, StaticCollisionComponent(0.5, 0.5, 0.5));
    coordinator.AddComponent(controlY, TagComponent("modelAxis"));
    m_ControlY = controlY;

    auto controlZ = coordinator.CreateEntity("modelAxis-z");
    auto renderZ = Render3DComponent(Configs::c_EditToolsModelPrefix + "scale-z", 0.1f, true);
    renderZ.rotation = glm::vec3(1.57f, 0.0f, 0.0f);
    coordinator.AddComponent(controlZ, renderZ);
    coordinator.AddComponent(controlZ, LocationComponent());
    coordinator.AddComponent(controlZ, StaticCollisionComponent(0.5, 0.5, 0.5));
    coordinator.AddComponent(controlZ, TagComponent("modelAxis"));
    m_ControlZ = controlZ;
}

void TransformControlsScale::onUpdate() {}

void TransformControlsScale::onDraw() {
    auto &gameCordinator = gameLayer().getCoordinator();
    auto &coordinator = toolsLayer().getCoordinator();

    auto &renderX = coordinator.GetComponent<Render3DComponent>(m_ControlX);
    auto &renderY = coordinator.GetComponent<Render3DComponent>(m_ControlY);
    auto &renderZ = coordinator.GetComponent<Render3DComponent>(m_ControlZ);

    if (m_ActiveControl == m_ControlX || (m_ActiveControl == c_NoEntity && m_HoveredControl == m_ControlX)) {
        renderX.scale = glm::vec3(0.06f);
    } else {
        renderX.scale = glm::vec3(0.05f);
    }

    if (m_ActiveControl == m_ControlY || (m_ActiveControl == c_NoEntity && m_HoveredControl == m_ControlY)) {
        renderY.scale = glm::vec3(0.06f);
    } else {
        renderY.scale = glm::vec3(0.05f);
    }

    if (m_ActiveControl == m_ControlZ || (m_ActiveControl == c_NoEntity && m_HoveredControl == m_ControlZ)) {
        renderZ.scale = glm::vec3(0.06f);
    } else {
        renderZ.scale = glm::vec3(0.05f);
    }

    renderX.updated = true;
    renderY.updated = true;
    renderZ.updated = true;

    auto &locationX = coordinator.GetComponent<LocationComponent>(m_ControlX);
    auto &locationY = coordinator.GetComponent<LocationComponent>(m_ControlY);
    auto &locationZ = coordinator.GetComponent<LocationComponent>(m_ControlZ);

    glm::vec3 rotEulerAngles = m_Model.rotation();
    glm::quat rot = glm::quat(rotEulerAngles);

    locationX.rotation = rotEulerAngles;
    locationY.rotation = rotEulerAngles;
    locationZ.rotation = rotEulerAngles;

    locationX.position = m_Model.position() + rot * glm::vec3(0.35f, 0.0f, 0.0f);
    locationY.position = m_Model.position() + rot * glm::vec3(0.0f, 0.35f, 0.0f);
    locationZ.position = m_Model.position() + rot * glm::vec3(0.0f, 0.0f, 0.35f);

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

void TransformControlsScale::onDetach() {}

void TransformControlsScale::onMouseEvent(MouseEvent &event) {
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

bool TransformControlsScale::handleSelection(Entity entity) {
    m_ActiveControl = entity;

    m_InitScale = m_Model.scale();
    m_InitMouseScreenPos = Application::get().getMousePicker().getPos();

    return entity == m_ControlX || entity == m_ControlY || entity == m_ControlZ;
}

void TransformControlsScale::hide() {
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

void TransformControlsScale::onTransform() {
    auto mousePos = Application::get().getMousePicker().ray();
    auto pos = Application::get().getCamera().positionVec();

    glm::vec3 dPosition = (mousePos - m_PrevMouseWorldPos) * glm::distance(pos, m_Model.position());
    glm::vec3 dRotation = dPosition;

    auto &camera = Application::get().getCamera();
    glm::vec4 screenPos = camera.projectionMatrix() * camera.viewMatrix() * glm::vec4(m_Model.position(), 1.0f);
    screenPos /= screenPos.w;
    screenPos.y *= -1.0f;
    screenPos = (screenPos + 1.0f) / 2.0f;

    const auto &viewport = Application::get().getRender().getViewport();

    glm::vec2 initMousePos = m_InitMouseScreenPos;
    initMousePos /= glm::vec2(viewport.width, viewport.height);

    glm::vec2 curMousePos = Application::get().getMousePicker().getPos();
    curMousePos /= glm::vec2(viewport.width, viewport.height);

    float initDist = glm::distance(glm::vec2(screenPos), initMousePos);
    float currDist = glm::distance(glm::vec2(screenPos), curMousePos);

    glm::vec3 scale = Math::rescale(m_InitScale, glm::vec3(1.0f), glm::vec3(currDist / initDist));

    if (m_ActiveControl != m_ControlX) {
        scale.x = m_InitScale.x;
    }
    if (m_ActiveControl != m_ControlY) {
        scale.y = m_InitScale.y;
    }
    if (m_ActiveControl != m_ControlZ) {
        scale.z = m_InitScale.z;
    }

    m_Model.scale(scale);
}

} // namespace Engine
