#include "DirectedLightDirector.hpp"

#include "Application.hpp"
#include "Configs.hpp"
#include "DirectedLightComponent.hpp"
#include "EditToolComponent.hpp"
#include "LocationComponent.hpp"
#include "ModelLoader.hpp"
#include "TagComponent.hpp"

#include "imgui/imgui.h"
#include <glm/gtx/matrix_decompose.hpp>

namespace Engine {

DirectedLightDirector::DirectedLightDirector(GameObjectModel &model) : m_Model(model) {}

void DirectedLightDirector::onAttach() {
    Application::get().getModels().RegisterModel(Configs::c_EditToolsModelPrefix + "Directional Light",
                                                 ModelLoader::load("./assets/models/box/arrow-y.fbx"));

    m_SunIcon = TextureLoader::loadTexture("assets/textures/icons/light.png");

    auto &coordinator = toolsLayer().getCoordinator();

    auto sun = coordinator.CreateEntity("Directional Light");
    coordinator.AddComponent(sun, LocationComponent(glm::vec3(0.0f, 3.0f, -5.0f)));
    coordinator.AddComponent(sun, TagComponent("Directional Light"));
    auto render = Render3DComponent(Configs::c_EditToolsModelPrefix + "Directional Light", 0.2f, true);
    render.rotation.x = 1.57f;
    coordinator.AddComponent(sun, render);
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

    auto model = LocationComponent::getFullTransform(entity, coordinator.GetComponentManager()) *
                 glm::toMat4(glm::quat(entityLight.light.rotation));

    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(model, scale, rotation, m_SunPosition, skew, perspective);

    auto &toolsCoordinator = toolsLayer().getCoordinator();

    auto &location = toolsCoordinator.GetComponent<LocationComponent>(m_Sun);
    location.position = m_SunPosition;
    location.rotation = glm::eulerAngles(rotation);
    toolsCoordinator.GetComponent<Render3DComponent>(m_Sun).updated = true;

    m_SunSelected = gameLayer().getCoordinator().HasComponent<DirectedLightComponent>(m_Model.entity());
    if (m_isVisible) {
        toolsLayer().getCoordinator().SetComponentActive<Render3DComponent>(m_Sun, m_SunSelected);
    }
}

void DirectedLightDirector::onDraw() {
    if (m_SunSelected) {
        return;
    }

    auto &camera = Application::get().getCamera();
    auto &viewport = Application::get().getRender().getViewport();

    glm::vec4 projection = camera.projectionMatrix() * camera.viewMatrix() * glm::vec4(m_SunPosition, 1.0f);
    projection /= projection.w;
    glm::vec2 screenPos = (glm::vec2(projection) + glm::vec2(1.0f)) * 0.5f * glm::vec2(viewport.width, viewport.height);

    Application::get().getRender().draw2D(m_SunIcon, screenPos, glm::vec2(50.0f, 50.0f),
                                          glm::vec4(0.75f, 0.75f, 0.75f, 1.0f), m_Sun);
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
    toolsLayer().getCoordinator().SetComponentActive<Render3DComponent>(m_Sun, true);
    BaseView::show();
}

void DirectedLightDirector::hide() {
    toolsLayer().getCoordinator().SetComponentActive<Render3DComponent>(m_Sun, false);
    BaseView::hide();
}

} // namespace Engine
