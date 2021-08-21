#include "DirectedLightDirector.hpp"

#include "Application.hpp"
#include "DirectedLightComponent.hpp"
#include "LocationComponent.hpp"
#include "ModelLoader.hpp"
#include "Render3DComponent.hpp"
#include "TagComponent.hpp"

#include "imgui/imgui.h"

namespace Engine {

DirectedLightDirector::DirectedLightDirector(GameObjectModel &model) : m_Model(model) {}

void DirectedLightDirector::onAttach() {
    Application::get().getModels().RegisterModel("sun", ModelLoader::loadModel("./assets/models/box/arrow-y.fbx"));

    auto &coordinator = gameLayer().getCoordinator();

    auto sun = coordinator.CreateEntity("sun");
    coordinator.AddComponent(sun, LocationComponent(glm::vec3(0.0f, 3.0f, -5.0f)));
    coordinator.AddComponent(sun, TagComponent("sun"));
    m_Sun = sun;

    m_Model.scaleChange$.addEventCallback([this, &coordinator](float scale, float prevScale) {
        if (m_Model.entity() != m_Sun) {
            return;
        }

        auto loghtArray = coordinator.GetComponentArray<DirectedLightComponent>();
        if (loghtArray->size() == 0) {
            return;
        }

        Entity entity = loghtArray->entities()[0];
        auto &entityLocation = coordinator.GetComponent<LocationComponent>(entity);
        auto &entityLight = coordinator.GetComponent<DirectedLightComponent>(entity);

        entityLight.light.ambient *= (1.0f / prevScale) * scale;
        entityLight.light.diffuse *= (1.0f / prevScale) * scale;
        entityLight.light.specular *= (1.0f / prevScale) * scale;
    });
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
        auto render = Render3DComponent("sun", 0.2f, true);
        render.rotation.x = 1.57f;
        coordinator.AddComponent(m_Sun, render);
    }

    BaseView::show();
}

void DirectedLightDirector::hide() {
    auto &coordinator = gameLayer().getCoordinator();
    if (coordinator.HasComponent<Render3DComponent>(m_Sun)) {
        coordinator.RemoveComponent<Render3DComponent>(m_Sun);
    }
    BaseView::hide();
}

} // namespace Engine
