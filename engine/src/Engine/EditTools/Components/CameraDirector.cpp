#include "CameraDirector.hpp"

#include "Application.hpp"
#include "CameraComponent.hpp"
#include "EditToolComponent.hpp"
#include "LocationComponent.hpp"
#include "ModelLoader.hpp"
#include "Render3DComponent.hpp"
#include "TagComponent.hpp"

#include "imgui/imgui.h"
#include <glm/gtx/quaternion.hpp>

namespace Engine {

CameraDirector::CameraDirector(GameObjectModel &model) : m_Model(model) {}

void CameraDirector::onAttach() {
    Application::get().getModels().RegisterModel("camera", ModelLoader::load("./assets/models/box/arrow-z.fbx"));

    auto &coordinator = gameLayer().getCoordinator();

    auto camera = coordinator.CreateEntity("camera");
    coordinator.AddComponent(camera, LocationComponent(glm::vec3(0.0f, 3.0f, 5.0f), glm::vec3(-0.5f, 0.0f, 0.0f)));
    coordinator.AddComponent(camera, TagComponent("camera"));
    coordinator.AddComponent(camera, EditToolComponent(true));
    m_Camera = camera;

    m_Model.positionChange$.addEventCallback([this, &coordinator](glm::vec3 position) {
        if (m_Model.entity() != m_Camera || coordinator.HasComponent<CameraComponent>(m_Model.entity())) {
            return;
        }

        auto cameraArray = coordinator.GetComponentArray<CameraComponent>();
        if (cameraArray->size() == 0) {
            return;
        }

        Entity entity = cameraArray->entities()[0];
        auto &entityLocation = coordinator.GetComponent<LocationComponent>(entity);
        auto &entityCamera = coordinator.GetComponent<CameraComponent>(entity);

        entityCamera.offset = glm::inverse(glm::quat(entityLocation.rotation)) * (position - entityLocation.position);
    });
    m_Model.rotationChange$.addEventCallback([this, &coordinator](glm::vec3 rotation) {
        if (m_Model.entity() != m_Camera || coordinator.HasComponent<CameraComponent>(m_Model.entity())) {
            return;
        }

        auto cameraArray = coordinator.GetComponentArray<CameraComponent>();
        if (cameraArray->size() == 0) {
            return;
        }

        Entity entity = cameraArray->entities()[0];
        auto &entityLocation = coordinator.GetComponent<LocationComponent>(entity);
        auto &entityCamera = coordinator.GetComponent<CameraComponent>(entity);

        entityCamera.rotation = rotation - entityLocation.rotation;
        // glm::eulerAngles(glm::quat(rotation) * glm::inverse(glm::quat(entityLocation.rotation)));
    });
}

void CameraDirector::onUpdate() {
    auto &camera = Application::get().getCamera();
    auto &coordinator = gameLayer().getCoordinator();

    if (!coordinator.HasComponent<CameraComponent>(m_Camera)) {
        auto cameraArray = coordinator.GetComponentArray<CameraComponent>();
        if (cameraArray->size() > 0) {
            Entity entity = cameraArray->entities()[0];
            auto &entityLocation = coordinator.GetComponent<LocationComponent>(entity);
            auto &entityCamera = coordinator.GetComponent<CameraComponent>(entity);

            glm::vec3 position = entityLocation.position + glm::quat(entityLocation.rotation) * entityCamera.offset;
            glm::quat rotation = glm::quat(entityLocation.rotation) * glm::quat(entityCamera.rotation);
            if (m_Model.entity() == m_Camera) {
                m_Model.position(position);
                m_Model.rotation(glm::eulerAngles(rotation));
            } else {
                auto &location = coordinator.GetComponent<LocationComponent>(m_Camera);
                location.position = position;
                location.rotation = glm::eulerAngles(rotation);
                coordinator.GetComponent<Render3DComponent>(m_Camera).updated = true;
            }
        } else {
            coordinator.AddComponent<CameraComponent>(m_Camera, CameraComponent());
        }
    }
}

void CameraDirector::show() {
    auto &coordinator = gameLayer().getCoordinator();
    if (!coordinator.HasComponent<Render3DComponent>(m_Camera)) {
        auto render = Render3DComponent("camera", 0.1f, true);
        render.rotation.y = 3.14f;
        coordinator.AddComponent(m_Camera, render);
    }
    BaseView::show();
}

void CameraDirector::hide() {
    auto &coordinator = gameLayer().getCoordinator();
    if (coordinator.HasComponent<Render3DComponent>(m_Camera)) {
        coordinator.RemoveComponent<Render3DComponent>(m_Camera);
    }
    BaseView::hide();
}

} // namespace Engine
