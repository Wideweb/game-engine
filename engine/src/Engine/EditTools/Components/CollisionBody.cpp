#include "CollisionBody.hpp"

#include "Application.hpp"
#include "File.hpp"
#include "LocationComponent.hpp"
#include "ModelFactory.hpp"

#include "imgui/imgui.h"

namespace Engine {

CollisionBody::CollisionBody(GameObjectModel &model) : m_Model(model) {}

void CollisionBody::onAttach() {
    auto vertexSrc = File::read("./shaders/overlay-vertex-shader.glsl");
    auto fragmentSrc = File::read("./shaders/overlay-fragment-shader.glsl");
    auto geometrySrc = File::read("./shaders/overlay-geometry-shader.glsl");
    m_Shader = std::make_shared<Shader>(vertexSrc, fragmentSrc, geometrySrc);

    Application::get().getModels().RegisterModel("collision-body", ModelFactory::createCube());

    auto &coordinator = toolsLayer().getCoordinator();

    auto collisionBody = coordinator.CreateEntity("collision-body");
    coordinator.AddComponent(collisionBody, LocationComponent());
    m_CollisionBody = collisionBody;

    m_Render = Render3DComponent("collision-body", 1.0f, true);
    m_Render.shader = m_Shader;

    m_Model.positionChange$.addEventCallback([this, &coordinator](glm::vec3 position) {
        auto &location = coordinator.GetComponent<LocationComponent>(m_CollisionBody);
        location.position = m_Model.position();

        if (coordinator.HasComponent<Render3DComponent>(m_CollisionBody)) {
            coordinator.GetComponent<Render3DComponent>(m_CollisionBody).updated = true;
        }
    });

    m_Model.rotationChange$.addEventCallback([this, &coordinator](glm::vec3 rotation) {
        auto &location = coordinator.GetComponent<LocationComponent>(m_CollisionBody);
        location.rotation = m_Model.rotation();

        if (coordinator.HasComponent<Render3DComponent>(m_CollisionBody)) {
            coordinator.GetComponent<Render3DComponent>(m_CollisionBody).updated = true;
        }
    });

    m_Model.collisionChange$.addEventCallback([this, &coordinator](const std::vector<glm::vec3> &vertices) {
        float width = std::abs(vertices[0].x);
        float height = std::abs(vertices[0].y);
        float depth = std::abs(vertices[0].z);

        if (coordinator.HasComponent<Render3DComponent>(m_CollisionBody)) {
            auto &render = coordinator.GetComponent<Render3DComponent>(m_CollisionBody);
            render.scale = glm::vec3(width, height, depth);
            render.updated = true;
        } else {
            m_Render.scale = glm::vec3(width, height, depth);
        }
    });
}

void CollisionBody::onUpdate() {}

void CollisionBody::show() {
    auto &coordinator = toolsLayer().getCoordinator();

    if (!coordinator.HasComponent<Render3DComponent>(m_CollisionBody)) {
        coordinator.AddComponent(m_CollisionBody, m_Render);
    }

    BaseView::show();
}

void CollisionBody::onDraw(int x, int y) {
    const auto &camera = Application::get().getCamera();

    m_Shader->bind();
    m_Shader->setMatrix4("u_view", camera.viewMatrix());
    m_Shader->setMatrix4("u_projection", camera.projectionMatrix());
    m_Shader->unbind();
}

void CollisionBody::hide() {
    auto &coordinator = toolsLayer().getCoordinator();

    if (coordinator.HasComponent<Render3DComponent>(m_CollisionBody)) {
        m_Render = coordinator.GetComponent<Render3DComponent>(m_CollisionBody);
        m_Render.instanced = false;
        coordinator.RemoveComponent<Render3DComponent>(m_CollisionBody);
    }

    BaseView::hide();
}

} // namespace Engine