#include "SpotLightSceneTool.hpp"

#include "Application.hpp"
#include "Configs.hpp"
#include "EditToolComponent.hpp"
#include "File.hpp"
#include "LocationComponent.hpp"
#include "Math.hpp"
#include "ModelFactory.hpp"
#include "ModelLoader.hpp"
#include "ParentComponent.hpp"
#include "SpotLightComponent.hpp"
#include "TagComponent.hpp"

#include "imgui/imgui.h"
#include <glm/gtx/matrix_decompose.hpp>

namespace Engine {

SpotLightSceneTool::SpotLightSceneTool(GameObjectModel &model) : m_Model(model) {}

void SpotLightSceneTool::onAttach() {
    Application::get().getModels().RegisterModel(Configs::c_EditToolsModelPrefix + "spot-light-sphere",
                                                 ModelFactory::createCircle(0.5f, 100, 0.001f));

    auto &coordinator = toolsLayer().getCoordinator();
    auto& materials = Application::get().getMaterials();

    auto sun = coordinator.CreateEntity("Spot Light");
    coordinator.AddComponent(sun, LocationComponent(glm::vec3(0.0f, 3.0f, -5.0f)));
    coordinator.AddComponent(sun, TagComponent("Spot Light"));
    m_Sun = sun;

    {
        auto sphere = coordinator.CreateEntity("Spot Light Frustum-Y");
        auto sphereLocation = LocationComponent(glm::vec3(0.0f));
        coordinator.AddComponent(sphere, sphereLocation);
        coordinator.AddComponent(sphere, ParentComponent(m_Sun));
        auto sphereRender = Render3DComponent(Configs::c_EditToolsModelPrefix + "spot-light-sphere", materials.meshMaterial.get());
        coordinator.AddComponent(sphere, sphereRender);
        m_SphereY = sphere;
    }

    {
        auto sphere = coordinator.CreateEntity("Spot Light Frustum-Z");
        auto sphereLocation = LocationComponent(glm::vec3(0.0f));
        sphereLocation.rotation.y = 1.57f;
        coordinator.AddComponent(sphere, sphereLocation);
        coordinator.AddComponent(sphere, ParentComponent(m_Sun));
        auto sphereRender = Render3DComponent(Configs::c_EditToolsModelPrefix + "spot-light-sphere", materials.meshMaterial.get());
        coordinator.AddComponent(sphere, sphereRender);
        m_SphereZ = sphere;
    }

    {
        auto sphere = coordinator.CreateEntity("Spot Light Sphere-X");
        auto sphereLocation = LocationComponent(glm::vec3(0.0f));
        sphereLocation.rotation.x = 1.57f;
        coordinator.AddComponent(sphere, sphereLocation);
        coordinator.AddComponent(sphere, ParentComponent(m_Sun));
        auto sphereRender = Render3DComponent(Configs::c_EditToolsModelPrefix + "spot-light-sphere", materials.meshMaterial.get());
        coordinator.AddComponent(sphere, sphereRender);
        m_SphereX = sphere;
    }
}

void SpotLightSceneTool::onUpdate() {
    auto &coordinator = gameLayer().getCoordinator();
    if (!coordinator.HasComponent<SpotLightComponent>(m_Model.entity())) {
        hide();
        return;
    }

    auto &entityLocation = coordinator.GetComponent<LocationComponent>(m_Model.entity());
    auto &entityLight = coordinator.GetComponent<SpotLightComponent>(m_Model.entity());
    auto &light = entityLight.light;

    auto model = LocationComponent::getFullTransform(m_Model.entity(), coordinator.GetComponentManager());

    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 position;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(model, scale, rotation, position, skew, perspective);

    auto &toolsCoordinator = toolsLayer().getCoordinator();

    auto &location = toolsCoordinator.GetComponent<LocationComponent>(m_Sun);
    location.position = position;
    location.rotation = glm::eulerAngles(rotation);
    location.updated = true;

    float ambientMax = std::max({light.ambient.r, light.ambient.g, light.ambient.b});
    float diffuseMax = std::max({light.diffuse.r, light.diffuse.g, light.diffuse.b});
    float specularMax = std::max({light.specular.r, light.specular.g, light.specular.b});

    float lightMaxEffect = 255.0f * (ambientMax + diffuseMax + specularMax) * light.intensity;
    glm::vec2 distanceVariants = Math::solve(light.quadratic, light.linear, light.constant - lightMaxEffect);
    float distance = distanceVariants.x < 0 ? distanceVariants.y : distanceVariants.x;

    {
        auto &sphereRender = toolsCoordinator.GetComponent<Render3DComponent>(m_SphereX);
        sphereRender.scale = glm::vec3(distance);
        sphereRender.updated = true;
    }

    {
        auto &sphereRender = toolsCoordinator.GetComponent<Render3DComponent>(m_SphereY);
        sphereRender.scale = glm::vec3(distance);
        sphereRender.updated = true;
    }

    {
        auto &sphereRender = toolsCoordinator.GetComponent<Render3DComponent>(m_SphereZ);
        sphereRender.scale = glm::vec3(distance);
        sphereRender.updated = true;
    }
}

void SpotLightSceneTool::show() {
    toolsLayer().getCoordinator().SetComponentActive<Render3DComponent>(m_SphereX, true);
    toolsLayer().getCoordinator().SetComponentActive<Render3DComponent>(m_SphereY, true);
    toolsLayer().getCoordinator().SetComponentActive<Render3DComponent>(m_SphereZ, true);
    BaseView::show();
}

void SpotLightSceneTool::hide() {
    toolsLayer().getCoordinator().SetComponentActive<Render3DComponent>(m_SphereX, false);
    toolsLayer().getCoordinator().SetComponentActive<Render3DComponent>(m_SphereY, false);
    toolsLayer().getCoordinator().SetComponentActive<Render3DComponent>(m_SphereZ, false);
    BaseView::hide();
}

} // namespace Engine
