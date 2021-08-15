#include "TransformControls.hpp"

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

TransformControls::TransformControls(GameObjectModel &model) : m_Model(model) {}

void TransformControls::onAttach() {
    auto &coordinator = toolsLayer().getCoordinator();

    Application::get().getModels().RegisterModel("arrow-x", ModelLoader::loadModel("./assets/models/box/arrow-x.fbx"));

    Application::get().getModels().RegisterModel("arrow-y", ModelLoader::loadModel("./assets/models/box/arrow-y.fbx"));

    Application::get().getModels().RegisterModel("arrow-z", ModelLoader::loadModel("./assets/models/box/arrow-z.fbx"));

    auto arrowX = coordinator.CreateEntity("arrow-x");
    auto arrowXLocation = LocationComponent();
    arrowXLocation.rotation = glm::vec3(0.0f, 1.57f, 0.0f);
    coordinator.AddComponent(arrowX, arrowXLocation);
    coordinator.AddComponent(arrowX, Render3DComponent("arrow-x", 0.1f, true));
    coordinator.AddComponent(arrowX, StaticCollisionComponent(0.5, 0.5, 0.5));
    coordinator.AddComponent(arrowX, TagComponent("arrow"));
    m_ArrowX = arrowX;

    auto arrowY = coordinator.CreateEntity("arrow-y");
    auto arrowYLocation = LocationComponent();
    arrowYLocation.rotation = glm::vec3(-1.57f, 0.0f, 0.0f);
    coordinator.AddComponent(arrowY, arrowYLocation);
    coordinator.AddComponent(arrowY, Render3DComponent("arrow-y", 0.1f, true));
    coordinator.AddComponent(arrowY, StaticCollisionComponent(0.5, 0.5, 0.5));
    coordinator.AddComponent(arrowY, TagComponent("arrow"));
    m_ArrowY = arrowY;

    auto arrowZ = coordinator.CreateEntity("arrow-z");
    auto arrowZLocation = LocationComponent();
    arrowZLocation.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    coordinator.AddComponent(arrowZ, arrowZLocation);
    coordinator.AddComponent(arrowZ, Render3DComponent("arrow-z", 0.1f, true));
    coordinator.AddComponent(arrowZ, StaticCollisionComponent(0.5, 0.5, 0.5));
    coordinator.AddComponent(arrowZ, TagComponent("arrow"));
    m_ArrowZ = arrowZ;

    Application::get().getModels().RegisterModel("arc-x", ModelLoader::loadModel("./assets/models/box/arc.obj"));
    {
        auto arcXModel = Application::get().getModels().GetModel<InstancedModel>("arc-x");
        auto &vertices = arcXModel->meshes[0].vertices;
        for (auto &vertex : vertices) {
            vertex.color = glm::vec3(1.0f, 0.0f, 0.0f);
        }
        arcXModel->update();
    }

    Application::get().getModels().RegisterModel("arc-y", ModelLoader::loadModel("./assets/models/box/arc.obj"));
    {
        auto arcYModel = Application::get().getModels().GetModel<InstancedModel>("arc-y");
        auto &vertices = arcYModel->meshes[0].vertices;
        for (auto &vertex : vertices) {
            vertex.color = glm::vec3(0.0f, 1.0f, 0.0f);
        }
        arcYModel->update();
    }

    Application::get().getModels().RegisterModel("arc-z", ModelLoader::loadModel("./assets/models/box/arc.obj"));
    {
        auto arcZModel = Application::get().getModels().GetModel<InstancedModel>("arc-z");
        auto &vertices = arcZModel->meshes[0].vertices;
        for (auto &vertex : vertices) {
            vertex.color = glm::vec3(0.0f, 0.0f, 1.0f);
        }
        arcZModel->update();
    }

    auto arcX = coordinator.CreateEntity("arc-x");
    auto arcXRender = Render3DComponent("arc-x", 0.1f, true);
    arcXRender.rotation = glm::vec3(1.57f, 1.57f, 0.0f);
    coordinator.AddComponent(arcX, arcXRender);
    coordinator.AddComponent(arcX, LocationComponent());
    coordinator.AddComponent(arcX, StaticCollisionComponent(0.25, 0.25, 0.25));
    coordinator.AddComponent(arcX, TagComponent("arc"));
    m_ArcX = arcX;

    auto arcY = coordinator.CreateEntity("arc-y");
    auto arcYRender = Render3DComponent("arc-y", 0.1f, true);
    arcYRender.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    coordinator.AddComponent(arcY, arcYRender);
    coordinator.AddComponent(arcY, LocationComponent());
    coordinator.AddComponent(arcY, StaticCollisionComponent(0.25, 0.25, 0.25));
    coordinator.AddComponent(arcY, TagComponent("arc"));
    m_ArcY = arcY;

    auto arcZ = coordinator.CreateEntity("arc-z");
    auto arcZRender = Render3DComponent("arc-z", 0.1f, true);
    arcZRender.rotation = glm::vec3(-1.57f, 0.0f, 0.0f);
    coordinator.AddComponent(arcZ, arcZRender);
    coordinator.AddComponent(arcZ, LocationComponent());
    coordinator.AddComponent(arcZ, StaticCollisionComponent(0.25, 0.25, 0.25));
    coordinator.AddComponent(arcZ, TagComponent("arc"));
    m_ArcZ = arcZ;

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

    {
        auto modelAxisX = coordinator.CreateEntity("modelAxis-x");
        auto modelAxisXRender = Render3DComponent("scale-x", 0.1f, true);
        modelAxisXRender.rotation = glm::vec3(0.0f, 0.0f, -1.57f);
        coordinator.AddComponent(modelAxisX, modelAxisXRender);
        coordinator.AddComponent(modelAxisX, LocationComponent());
        coordinator.AddComponent(modelAxisX, StaticCollisionComponent(0.5, 0.5, 0.5));
        coordinator.AddComponent(modelAxisX, TagComponent("modelAxis"));
        m_ModelAxisX = modelAxisX;

        auto modelAxisY = coordinator.CreateEntity("modelAxis-y");
        auto modelAxisYRender = Render3DComponent("scale-y", 0.1f, true);
        modelAxisYRender.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
        coordinator.AddComponent(modelAxisY, modelAxisYRender);
        coordinator.AddComponent(modelAxisY, LocationComponent());
        coordinator.AddComponent(modelAxisY, StaticCollisionComponent(0.5, 0.5, 0.5));
        coordinator.AddComponent(modelAxisY, TagComponent("modelAxis"));
        m_ModelAxisY = modelAxisY;

        auto modelAxisZ = coordinator.CreateEntity("modelAxis-z");
        auto modelAxisZRender = Render3DComponent("scale-z", 0.1f, true);
        modelAxisZRender.rotation = glm::vec3(1.57f, 0.0f, 0.0f);
        coordinator.AddComponent(modelAxisZ, modelAxisZRender);
        coordinator.AddComponent(modelAxisZ, LocationComponent());
        coordinator.AddComponent(modelAxisZ, StaticCollisionComponent(0.5, 0.5, 0.5));
        coordinator.AddComponent(modelAxisZ, TagComponent("modelAxis"));
        m_ModelAxisZ = modelAxisZ;
    }
}

void TransformControls::onUpdate() {}

void TransformControls::onDraw(int, int) {
    auto &gameCordinator = gameLayer().getCoordinator();
    auto &coordinator = toolsLayer().getCoordinator();

    {
        auto &renderX = coordinator.GetComponent<Render3DComponent>(m_ArrowX);
        auto &renderY = coordinator.GetComponent<Render3DComponent>(m_ArrowY);
        auto &renderZ = coordinator.GetComponent<Render3DComponent>(m_ArrowZ);

        renderX.scale = 0.1f;
        renderY.scale = 0.1f;
        renderZ.scale = 0.1f;

        renderX.updated = true;
        renderY.updated = true;
        renderZ.updated = true;

        auto &arrowXLocation = coordinator.GetComponent<LocationComponent>(m_ArrowX);
        auto &arrowYLocation = coordinator.GetComponent<LocationComponent>(m_ArrowY);
        auto &arrowZLocation = coordinator.GetComponent<LocationComponent>(m_ArrowZ);

        arrowXLocation.position = m_Model.position() + glm::vec3(2.0f, 0.0f, 0.0f);
        arrowYLocation.position = m_Model.position() + glm::vec3(0.0f, 2.0f, 0.0f);
        arrowZLocation.position = m_Model.position() + glm::vec3(0.0f, 0.0f, 2.0f);

        arrowXLocation.updated = true;
        arrowYLocation.updated = true;
        arrowZLocation.updated = true;

        auto &collisionX = coordinator.GetComponent<StaticCollisionComponent>(m_ArrowX);
        auto &collisionY = coordinator.GetComponent<StaticCollisionComponent>(m_ArrowY);
        auto &collisionZ = coordinator.GetComponent<StaticCollisionComponent>(m_ArrowZ);

        collisionX.updated = true;
        collisionY.updated = true;
        collisionZ.updated = true;
    }

    {
        auto &renderX = coordinator.GetComponent<Render3DComponent>(m_ArcX);
        auto &renderY = coordinator.GetComponent<Render3DComponent>(m_ArcY);
        auto &renderZ = coordinator.GetComponent<Render3DComponent>(m_ArcZ);

        renderX.scale = 0.1f;
        renderY.scale = 0.1f;
        renderZ.scale = 0.1f;

        renderX.updated = true;
        renderY.updated = true;
        renderZ.updated = true;

        auto &arcXLocation = coordinator.GetComponent<LocationComponent>(m_ArcX);
        auto &arcYLocation = coordinator.GetComponent<LocationComponent>(m_ArcY);
        auto &arcZLocation = coordinator.GetComponent<LocationComponent>(m_ArcZ);

        arcXLocation.position = m_Model.position() + glm::vec3(1.5f, 0.5f, 0.0f);
        arcYLocation.position = m_Model.position() + glm::vec3(0.0f, 1.5f, -0.5f);
        arcZLocation.position = m_Model.position() + glm::vec3(0.0f, -0.5f, 1.5f);

        arcXLocation.updated = true;
        arcYLocation.updated = true;
        arcZLocation.updated = true;

        auto &collisionX = coordinator.GetComponent<StaticCollisionComponent>(m_ArcX);
        auto &collisionY = coordinator.GetComponent<StaticCollisionComponent>(m_ArcY);
        auto &collisionZ = coordinator.GetComponent<StaticCollisionComponent>(m_ArcZ);

        collisionX.updated = true;
        collisionY.updated = true;
        collisionZ.updated = true;
    }

    {
        auto &renderX = coordinator.GetComponent<Render3DComponent>(m_ModelAxisX);
        auto &renderY = coordinator.GetComponent<Render3DComponent>(m_ModelAxisY);
        auto &renderZ = coordinator.GetComponent<Render3DComponent>(m_ModelAxisZ);

        renderX.scale = 0.05f;
        renderY.scale = 0.05f;
        renderZ.scale = 0.05f;

        renderX.updated = true;
        renderY.updated = true;
        renderZ.updated = true;

        auto &locationX = coordinator.GetComponent<LocationComponent>(m_ModelAxisX);
        auto &locationY = coordinator.GetComponent<LocationComponent>(m_ModelAxisY);
        auto &locationZ = coordinator.GetComponent<LocationComponent>(m_ModelAxisZ);

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

        auto &collisionX = coordinator.GetComponent<StaticCollisionComponent>(m_ModelAxisX);
        auto &collisionY = coordinator.GetComponent<StaticCollisionComponent>(m_ModelAxisY);
        auto &collisionZ = coordinator.GetComponent<StaticCollisionComponent>(m_ModelAxisZ);

        collisionX.updated = true;
        collisionY.updated = true;
        collisionZ.updated = true;
    }
}

void TransformControls::onDetach() {}

void TransformControls::onMouseEvent(MouseEvent &event) {
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

bool TransformControls::handleSelection(Entity entity) {
    if (entity != m_ArrowX && entity != m_ArrowY && entity != m_ArrowZ && entity != m_ArcX && entity != m_ArcY &&
        entity != m_ArcZ && entity != m_ModelAxisX && entity != m_ModelAxisY && entity != m_ModelAxisZ) {
        return false;
    }

    m_ActiveControl = entity;

    m_InitScale = m_Model.scale();
    m_InitMouseScreenPos = Application::get().getMousePicker().getPos();

    return true;
}

void TransformControls::hide() {
    auto &coordinator = toolsLayer().getCoordinator();

    {
        auto &renderX = coordinator.GetComponent<Render3DComponent>(m_ArrowX);
        auto &renderY = coordinator.GetComponent<Render3DComponent>(m_ArrowY);
        auto &renderZ = coordinator.GetComponent<Render3DComponent>(m_ArrowZ);

        renderX.scale = 0.0f;
        renderY.scale = 0.0f;
        renderZ.scale = 0.0f;

        renderX.updated = true;
        renderY.updated = true;
        renderZ.updated = true;
    }

    {
        auto &renderX = coordinator.GetComponent<Render3DComponent>(m_ArcX);
        auto &renderY = coordinator.GetComponent<Render3DComponent>(m_ArcY);
        auto &renderZ = coordinator.GetComponent<Render3DComponent>(m_ArcZ);

        renderX.scale = 0.0f;
        renderY.scale = 0.0f;
        renderZ.scale = 0.0f;

        renderX.updated = true;
        renderY.updated = true;
        renderZ.updated = true;
    }

    {
        auto &renderX = coordinator.GetComponent<Render3DComponent>(m_ModelAxisX);
        auto &renderY = coordinator.GetComponent<Render3DComponent>(m_ModelAxisY);
        auto &renderZ = coordinator.GetComponent<Render3DComponent>(m_ModelAxisZ);

        renderX.scale = 0.0f;
        renderY.scale = 0.0f;
        renderZ.scale = 0.0f;

        renderX.updated = true;
        renderY.updated = true;
        renderZ.updated = true;
    }

    BaseView::hide();
}

void TransformControls::onTransform() {
    auto mousePos = Application::get().getMousePicker().ray();
    auto pos = Application::get().getCamera().positionVec();

    glm::vec3 dPosition = (mousePos - m_PrevMouseWorldPos) * glm::distance(pos, m_Model.position());
    glm::vec3 dRotation = dPosition;

    if (m_ActiveControl != m_ArrowX) {
        dPosition.x = 0;
    }
    if (m_ActiveControl != m_ArrowY) {
        dPosition.y = 0;
    }
    if (m_ActiveControl != m_ArrowZ) {
        dPosition.z = 0;
    }
    m_Model.position(m_Model.position() + dPosition);

    if (m_ActiveControl == m_ModelAxisX || m_ActiveControl == m_ModelAxisY || m_ActiveControl == m_ModelAxisZ) {
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

    if (m_ActiveControl == m_ArcX) {
        dRotation.x = -dRotation.y + dRotation.z;
        dRotation.y = 0;
        dRotation.z = 0;
    } else if (m_ActiveControl == m_ArcY) {
        dRotation.y = -(dRotation.x + dRotation.z);
        dRotation.x = 0;
        dRotation.z = 0;
    } else if (m_ActiveControl == m_ArcZ) {
        dRotation.z = dRotation.x + -dRotation.y;
        dRotation.x = 0;
        dRotation.y = 0;
    } else {
        dRotation = glm::vec3(0.0f);
    }
    m_Model.rotation(glm::eulerAngles(glm::quat(dRotation) * glm::quat(m_Model.rotation())));
}

} // namespace Engine
