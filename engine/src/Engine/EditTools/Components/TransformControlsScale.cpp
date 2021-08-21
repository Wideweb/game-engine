#include "TransformControlsScale.hpp"

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

TransformControlsScale::TransformControlsScale(GameObjectModel &model) : m_Model(model) {}

void TransformControlsScale::onAttach() {
    auto &coordinator = toolsLayer().getCoordinator();

    {
        Application::get().getModels().RegisterModel("scale-x",
                                                     ModelLoader::loadModel("./assets/models/box/scale.obj"));

        auto model = Application::get().getModels().GetModel<InstancedModel>("scale-x");
        auto &vertices = model->meshes[0].vertices;
        for (auto &vertex : vertices) {
            vertex.color = glm::vec3(1.0f, 0.0f, 0.0f);
        }
        model->update();
    }

    {
        Application::get().getModels().RegisterModel("scale-y",
                                                     ModelLoader::loadModel("./assets/models/box/scale.obj"));

        auto model = Application::get().getModels().GetModel<InstancedModel>("scale-y");
        auto &vertices = model->meshes[0].vertices;
        for (auto &vertex : vertices) {
            vertex.color = glm::vec3(0.0f, 1.0f, 0.0f);
        }
        model->update();
    }

    {
        Application::get().getModels().RegisterModel("scale-z",
                                                     ModelLoader::loadModel("./assets/models/box/scale.obj"));

        auto model = Application::get().getModels().GetModel<InstancedModel>("scale-z");
        auto &vertices = model->meshes[0].vertices;
        for (auto &vertex : vertices) {
            vertex.color = glm::vec3(0.0f, 0.0f, 1.0f);
        }
        model->update();
    }

    auto controlX = coordinator.CreateEntity("modelAxis-x");
    auto renderX = Render3DComponent("scale-x", 0.1f, true);
    renderX.rotation = glm::vec3(0.0f, 0.0f, -1.57f);
    coordinator.AddComponent(controlX, renderX);
    coordinator.AddComponent(controlX, LocationComponent());
    coordinator.AddComponent(controlX, StaticCollisionComponent(0.5, 0.5, 0.5));
    coordinator.AddComponent(controlX, TagComponent("modelAxis"));
    m_ControlX = controlX;

    auto controlY = coordinator.CreateEntity("modelAxis-y");
    auto renderY = Render3DComponent("scale-y", 0.1f, true);
    renderY.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    coordinator.AddComponent(controlY, renderY);
    coordinator.AddComponent(controlY, LocationComponent());
    coordinator.AddComponent(controlY, StaticCollisionComponent(0.5, 0.5, 0.5));
    coordinator.AddComponent(controlY, TagComponent("modelAxis"));
    m_ControlY = controlY;

    auto controlZ = coordinator.CreateEntity("modelAxis-z");
    auto renderZ = Render3DComponent("scale-z", 0.1f, true);
    renderZ.rotation = glm::vec3(1.57f, 0.0f, 0.0f);
    coordinator.AddComponent(controlZ, renderZ);
    coordinator.AddComponent(controlZ, LocationComponent());
    coordinator.AddComponent(controlZ, StaticCollisionComponent(0.5, 0.5, 0.5));
    coordinator.AddComponent(controlZ, TagComponent("modelAxis"));
    m_ControlZ = controlZ;
}

void TransformControlsScale::onUpdate() {}

void TransformControlsScale::onDraw(int, int) {
    auto &gameCordinator = gameLayer().getCoordinator();
    auto &coordinator = toolsLayer().getCoordinator();

    auto &renderX = coordinator.GetComponent<Render3DComponent>(m_ControlX);
    auto &renderY = coordinator.GetComponent<Render3DComponent>(m_ControlY);
    auto &renderZ = coordinator.GetComponent<Render3DComponent>(m_ControlZ);

    renderX.scale = 0.05f;
    renderY.scale = 0.05f;
    renderZ.scale = 0.05f;

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
        if (m_ActiveControl != c_NoEntity) {
            onTransform();
            event.handled = true;
        }
    }

    if (event.type == EventType::MouseUp) {
        m_ActiveControl = c_NoEntity;
    }

    m_PrevMouseWorldPos = Application::get().getMousePicker().ray();
}

bool TransformControlsScale::handleSelection(Entity entity) {
    if (entity != m_ControlX && entity != m_ControlY && entity != m_ControlZ) {
        return false;
    }

    m_ActiveControl = entity;

    m_InitScale = m_Model.scale();
    m_InitMouseScreenPos = Application::get().getMousePicker().getPos();

    return true;
}

void TransformControlsScale::hide() {
    auto &coordinator = toolsLayer().getCoordinator();

    auto &renderX = coordinator.GetComponent<Render3DComponent>(m_ControlX);
    auto &renderY = coordinator.GetComponent<Render3DComponent>(m_ControlY);
    auto &renderZ = coordinator.GetComponent<Render3DComponent>(m_ControlZ);

    renderX.scale = 0.0f;
    renderY.scale = 0.0f;
    renderZ.scale = 0.0f;

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

    m_Model.scale(m_InitScale * (currDist / initDist));
}

} // namespace Engine
