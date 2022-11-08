#include "CameraDirector.hpp"

#include "Application.hpp"
#include "CameraComponent.hpp"
#include "Configs.hpp"
#include "File.hpp"
#include "LocationComponent.hpp"
#include "ModelFactory.hpp"
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

    Application::get().getModels().RegisterModel(Configs::c_EditToolsModelPrefix + "camera-frustum",
                                                 ModelFactory::createFrastum(glm::radians(45.0f), 0.1f, 50.0f));

    m_CameraIcon = TextureLoader::loadTexture("assets/textures/icons/camera.png");

    auto &toolsCoordinator = toolsLayer().getCoordinator();
    auto& materials = Application::get().getMaterials();

    auto camera = toolsCoordinator.CreateEntity("Camera");
    toolsCoordinator.AddComponent(camera, LocationComponent(glm::vec3(0.0f, 3.0f, 5.0f), glm::vec3(-0.5f, 0.0f, 0.0f)));
    toolsCoordinator.AddComponent(camera, TagComponent("Camera"));
    auto render = Render3DComponent(Configs::c_EditToolsModelPrefix + "Camera", materials.sceneToolMaterial.get(), 0.1f);
    render.rotation.y = 3.14f;
    toolsCoordinator.AddComponent(camera, render);
    m_Camera = camera;

    auto frustum = toolsCoordinator.CreateEntity("Camera Frustum");
    auto frustumLocation = LocationComponent(glm::vec3(0.0f));
    frustumLocation.rotation.y = 3.14f;
    toolsCoordinator.AddComponent(frustum, frustumLocation);
    toolsCoordinator.AddComponent(frustum, ParentComponent(m_Camera));
    auto frustumRender = Render3DComponent(Configs::c_EditToolsModelPrefix + "camera-frustum", materials.cubeEdgesMaterial.get(), 1.0);
    toolsCoordinator.AddComponent(frustum, frustumRender);
    m_Frustum = frustum;
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
        toolsLayer().getCoordinator().SetComponentActive<Render3DComponent>(m_Frustum, m_CameraSelected);

        float tangent = std::tanf(camera.getFieldOfView() / 2.0f);

        float fronDelta = camera.getZNear() * tangent;
        float backDelta = camera.getZFar() * tangent;

        auto frustumModel =
            Application::get().getModels().GetModel<InstancedModel>(Configs::c_EditToolsModelPrefix + "camera-frustum");
        auto &frustumVertices = frustumModel->meshes[0].vertices;

        frustumVertices[0].position = glm::vec3(-fronDelta, -fronDelta, camera.getZNear());
        frustumVertices[1].position = glm::vec3(-fronDelta, fronDelta, camera.getZNear());
        frustumVertices[2].position = glm::vec3(fronDelta, fronDelta, camera.getZNear());
        frustumVertices[3].position = glm::vec3(fronDelta, -fronDelta, camera.getZNear());

        frustumVertices[4].position = glm::vec3(-backDelta, -backDelta, camera.getZFar());
        frustumVertices[5].position = glm::vec3(-backDelta, backDelta, camera.getZFar());
        frustumVertices[6].position = glm::vec3(backDelta, backDelta, camera.getZFar());
        frustumVertices[7].position = glm::vec3(backDelta, -backDelta, camera.getZFar());

        frustumModel->update();
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
    toolsLayer().getCoordinator().SetComponentActive<Render3DComponent>(m_Frustum, true);
    BaseView::show();
}

void CameraDirector::hide() {
    toolsLayer().getCoordinator().SetComponentActive<Render3DComponent>(m_Camera, false);
    toolsLayer().getCoordinator().SetComponentActive<Render3DComponent>(m_Frustum, false);
    BaseView::hide();
}

} // namespace Engine
