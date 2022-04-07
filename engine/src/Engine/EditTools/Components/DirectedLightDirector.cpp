#include "DirectedLightDirector.hpp"

#include "Application.hpp"
#include "DirectedLightComponent.hpp"
#include "EditToolComponent.hpp"
#include "LocationComponent.hpp"
#include "ModelLoader.hpp"
#include "TagComponent.hpp"

#include "imgui/imgui.h"

namespace Engine {

DirectedLightDirector::DirectedLightDirector(GameObjectModel &model) : m_Model(model) {}

void DirectedLightDirector::onAttach() {
    Application::get().getModels().RegisterModel("EditTool::Directional Light",
                                                 ModelLoader::load("./assets/models/box/arrow-y.fbx"));

    auto &coordinator = toolsLayer().getCoordinator();

    auto sun = coordinator.CreateEntity("Directional Light");
    coordinator.AddComponent(sun, LocationComponent(glm::vec3(0.0f, 3.0f, -5.0f)));
    coordinator.AddComponent(sun, TagComponent("Directional Light"));
    m_Sun = sun;
}

void DirectedLightDirector::onUpdate() {
    auto &coordinator = gameLayer().getCoordinator();
    auto lightArray = coordinator.GetComponentArray<DirectedLightComponent>();

    if (lightArray->size() <= 0) {
        hide();
        return;
    }

    Entity entity = lightArray->entities()[0];
    auto &entityLocation = coordinator.GetComponent<LocationComponent>(entity);
    auto &entityLight = coordinator.GetComponent<DirectedLightComponent>(entity);

    glm::vec3 position = entityLocation.position;
    glm::quat rotation = glm::quat(entityLocation.rotation) * glm::quat(entityLight.light.rotation);

    auto &toolsCoordinator = toolsLayer().getCoordinator();

    auto &location = toolsCoordinator.GetComponent<LocationComponent>(m_Sun);
    location.position = position;
    location.rotation = glm::eulerAngles(rotation);
    toolsCoordinator.GetComponent<Render3DComponent>(m_Sun).updated = true;
}

bool DirectedLightDirector::handleSelection(Entity entity) {
    if (m_Sun != entity) {
        return false;
    }

    auto lightArray = gameLayer().getCoordinator().GetComponentArray<DirectedLightComponent>();
    if (lightArray->size() > 0) {
        m_Model.setEntity(lightArray->entities()[0]);
        return true;
    }

    return false;
}

void DirectedLightDirector::show() {
    auto &coordinator = toolsLayer().getCoordinator();
    if (!coordinator.HasComponent<Render3DComponent>(m_Sun)) {
        auto render = Render3DComponent("EditTool::Directional Light", 0.2f, true);
        render.rotation.x = 1.57f;
        coordinator.AddComponent(m_Sun, render);
    }

    BaseView::show();
}

void DirectedLightDirector::hide() {
    auto &coordinator = toolsLayer().getCoordinator();
    if (coordinator.HasComponent<Render3DComponent>(m_Sun)) {
        coordinator.RemoveComponent<Render3DComponent>(m_Sun);
    }
    BaseView::hide();
}

} // namespace Engine
