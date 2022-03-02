#include "DirectedLightDirector.hpp"

#include "Application.hpp"
#include "DirectedLightComponent.hpp"
#include "LocationComponent.hpp"
#include "ModelLoader.hpp"
#include "TagComponent.hpp"

#include "imgui/imgui.h"

namespace Engine {

DirectedLightDirector::DirectedLightDirector(GameObjectModel &model) : m_Model(model) {}

void DirectedLightDirector::onAttach() {
    Application::get().getModels().RegisterModel("sun", ModelLoader::load("./assets/models/box/arrow-y.fbx"));

    auto &coordinator = gameLayer().getCoordinator();

    auto sun = coordinator.CreateEntity("sun");
    coordinator.AddComponent(sun, LocationComponent(glm::vec3(0.0f, 3.0f, -5.0f)));
    coordinator.AddComponent(sun, TagComponent("sun"));
    m_Sun = sun;

    m_Render = Render3DComponent("sun", 0.2f, true);
    m_Render.rotation.x = 1.57f;
}

void DirectedLightDirector::onUpdate() {
    auto &coordinator = gameLayer().getCoordinator();
    auto lightArray = coordinator.GetComponentArray<DirectedLightComponent>();
    if (lightArray->size() == 0) {
        coordinator.AddComponent(m_Sun, DirectedLightComponent());
    }
}

void DirectedLightDirector::show() {
    auto &coordinator = gameLayer().getCoordinator();

    if (coordinator.HasComponent<Render3DComponent>(m_Sun)) {
        return;
    }

    auto lightArray = coordinator.GetComponentArray<DirectedLightComponent>();
    if (lightArray->size() > 0 && lightArray->entities()[0] == m_Sun) {
        coordinator.AddComponent(m_Sun, m_Render);
    }

    BaseView::show();
}

void DirectedLightDirector::hide() {
    auto &coordinator = gameLayer().getCoordinator();
    if (coordinator.HasComponent<Render3DComponent>(m_Sun)) {
        m_Render = coordinator.GetComponent<Render3DComponent>(m_Sun);
        m_Render.instanced = false;
        coordinator.RemoveComponent<Render3DComponent>(m_Sun);
    }
    BaseView::hide();
}

} // namespace Engine
