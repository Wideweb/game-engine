#include "CameraDirector.hpp"

#include "Application.hpp"
#include "CameraComponent.hpp"
#include "LocationComponent.hpp"
#include "ModelLoader.hpp"
#include "Render3DComponent.hpp"
#include "TagComponent.hpp"

#include "imgui/imgui.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace Engine {

CameraDirector::CameraDirector(GameObjectModel &model) : m_Model(model) {}

void CameraDirector::onAttach() {
    Application::get().getModels().RegisterModel("EditTool::Camera",
                                                 ModelLoader::load("./assets/models/box/arrow-z.fbx"));

    auto &toolsCoordinator = toolsLayer().getCoordinator();
    auto &gameCoordinator = gameLayer().getCoordinator();

    auto camera = toolsCoordinator.CreateEntity("Camera");
    toolsCoordinator.AddComponent(camera, LocationComponent(glm::vec3(0.0f, 3.0f, 5.0f), glm::vec3(-0.5f, 0.0f, 0.0f)));
    toolsCoordinator.AddComponent(camera, TagComponent("Camera"));
    m_Camera = camera;
}

void CameraDirector::onUpdate() {
    auto &camera = Application::get().getCamera();
    auto &coordinator = gameLayer().getCoordinator();

    auto cameraArray = coordinator.GetComponentArray<CameraComponent>();
    if (cameraArray->size() <= 0) {
        hide();
        return;
    }

    Entity entity = cameraArray->entities()[0];
    auto &entityLocation = coordinator.GetComponent<LocationComponent>(entity);
    auto &entityCamera = coordinator.GetComponent<CameraComponent>(entity);

    auto model = LocationComponent::getFullTransform(entity, coordinator.GetComponentManager()) *
                 glm::translate(glm::mat4(1.0), entityCamera.offset) * glm::toMat4(glm::quat(entityCamera.rotation));

    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 position;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(model, scale, rotation, position, skew, perspective);

    auto &toolsCoordinator = toolsLayer().getCoordinator();

    auto &location = toolsCoordinator.GetComponent<LocationComponent>(m_Camera);
    location.position = position;
    location.rotation = glm::eulerAngles(rotation);
    toolsCoordinator.GetComponent<Render3DComponent>(m_Camera).updated = true;
}

bool CameraDirector::handleSelection(Entity entity) {
    if (m_Camera != entity) {
        return false;
    }

    auto cameraArray = gameLayer().getCoordinator().GetComponentArray<CameraComponent>();
    if (cameraArray->size() > 0) {
        m_Model.setEntity(cameraArray->entities()[0]);
        return true;
    }

    return false;
}

void CameraDirector::show() {
    auto &coordinator = toolsLayer().getCoordinator();
    if (!coordinator.HasComponent<Render3DComponent>(m_Camera)) {
        auto render = Render3DComponent("EditTool::Camera", 0.1f, true);
        render.rotation.y = 3.14f;
        coordinator.AddComponent(m_Camera, render);
    }

    BaseView::show();
}

void CameraDirector::hide() {
    auto &coordinator = toolsLayer().getCoordinator();
    if (coordinator.HasComponent<Render3DComponent>(m_Camera)) {
        coordinator.RemoveComponent<Render3DComponent>(m_Camera);
    }
    BaseView::hide();
}

} // namespace Engine
