#include "CameraDirector.hpp"

#include "Application.hpp"
#include "CameraComponent.hpp"
#include "Configs.hpp"
#include "LocationComponent.hpp"
#include "ModelLoader.hpp"
#include "Render3DComponent.hpp"
#include "TagComponent.hpp"
#include "TextureLoader.hpp"

#include "imgui/imgui.h"
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Engine {

CameraDirector::CameraDirector(GameObjectModel &model) : m_Model(model) {}

void CameraDirector::onAttach() {
    Application::get().getModels().RegisterModel(Configs::c_EditToolsModelPrefix + "Camera",
                                                 ModelLoader::load("./assets/models/box/arrow-z.fbx"));

    m_CameraIcon = TextureLoader::loadTexture("assets/textures/icons/camera.png");

    auto &toolsCoordinator = toolsLayer().getCoordinator();
    auto &gameCoordinator = gameLayer().getCoordinator();

    auto camera = toolsCoordinator.CreateEntity("Camera");
    toolsCoordinator.AddComponent(camera, LocationComponent(glm::vec3(0.0f, 3.0f, 5.0f), glm::vec3(-0.5f, 0.0f, 0.0f)));
    toolsCoordinator.AddComponent(camera, TagComponent("Camera"));
    auto render = Render3DComponent(Configs::c_EditToolsModelPrefix + "Camera", 0.1f, true);
    render.rotation.y = 3.14f;
    toolsCoordinator.AddComponent(camera, render);
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

    auto model = LocationComponent::getFullTransform(entity, coordinator.GetComponentManager());

    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(model, scale, rotation, m_CameraPosition, skew, perspective);

    auto &toolsCoordinator = toolsLayer().getCoordinator();

    auto &location = toolsCoordinator.GetComponent<LocationComponent>(m_Camera);
    location.position = m_CameraPosition;
    location.rotation = glm::eulerAngles(rotation);
    toolsCoordinator.GetComponent<Render3DComponent>(m_Camera).updated = true;

    m_CameraSelected = gameLayer().getCoordinator().HasComponent<CameraComponent>(m_Model.entity());
    if (m_isVisible) {
        toolsLayer().getCoordinator().SetComponentActive<Render3DComponent>(m_Camera, m_CameraSelected);
    }
}

void CameraDirector::onDraw() {
    if (m_CameraSelected) {
        return;
    }

    auto &camera = Application::get().getCamera();
    auto &viewport = Application::get().getRender().getViewport();

    glm::vec4 projection = camera.projectionMatrix() * camera.viewMatrix() * glm::vec4(m_CameraPosition, 1.0f);
    projection /= projection.w;
    glm::vec2 screenPos = (glm::vec2(projection) + glm::vec2(1.0f)) * 0.5f * glm::vec2(viewport.width, viewport.height);

    Application::get().getRender().draw2D(m_CameraIcon, screenPos, glm::vec2(50.0f, 50.0f),
                                          glm::vec4(0.75f, 0.75f, 0.75f, 1.0f), m_Camera);
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
    toolsLayer().getCoordinator().SetComponentActive<Render3DComponent>(m_Camera, true);
    BaseView::show();
}

void CameraDirector::hide() {
    toolsLayer().getCoordinator().SetComponentActive<Render3DComponent>(m_Camera, false);
    BaseView::hide();
}

} // namespace Engine
