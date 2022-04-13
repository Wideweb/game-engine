#include "MeshBody.hpp"

#include "Application.hpp"
#include "EditToolComponent.hpp"
#include "File.hpp"
#include "LocationComponent.hpp"
#include "ParentComponent.hpp"
#include "Render3DComponent.hpp"

#include "imgui/imgui.h"

namespace Engine {

MeshBody::MeshBody(GameObjectModel &model) : m_Model(model) {}

void MeshBody::onAttach() {
    auto vertexSrc = File::read("./shaders/overlay-vertex-shader.glsl");
    auto fragmentSrc = File::read("./shaders/overlay-fragment-shader.glsl");
    auto geometrySrc = File::read("./shaders/overlay-geometry-shader.glsl");
    m_Shader = std::make_shared<Shader>(vertexSrc, fragmentSrc, geometrySrc);

    m_Shader->bind();
    m_Shader->setInt("u_useColor", 1);
    m_Shader->setFloat3("u_color", glm::vec3(0.0f, 1.0f, 0.0f));
    m_Shader->unbind();

    auto &coordinator = gameLayer().getCoordinator();

    m_MeshBody = coordinator.CreateEntity("mesh-body");
    coordinator.AddComponent(m_MeshBody, LocationComponent());
    coordinator.AddComponent(m_MeshBody, EditToolComponent());
    coordinator.AddComponent(m_MeshBody, ParentComponent());

    auto render = Render3DComponent("", 1.0f, true);
    render.shader = m_Shader;
    coordinator.AddComponent(m_MeshBody, render);

    hide();

    m_Model.entityChange$.addEventCallback([&](Entity entity) {
        if (!coordinator.HasComponent<Render3DComponent>(entity) ||
            coordinator.HasComponent<EditToolComponent>(entity)) {
            hide();
            return;
        }

        coordinator.GetComponent<ParentComponent>(m_MeshBody).setEntity(entity);
        coordinator.GetComponent<Render3DComponent>(m_MeshBody).model =
            coordinator.GetComponent<Render3DComponent>(entity).model;

        show();
    });
}

void MeshBody::onUpdate() {}

void MeshBody::show() {
    auto &coordinator = gameLayer().getCoordinator();

    coordinator.SetComponentActive<Render3DComponent>(m_MeshBody, true);
    coordinator.SetComponentActive<ParentComponent>(m_MeshBody, true);

    BaseView::show();
}

void MeshBody::onDraw(int x, int y) {
    const auto &camera = Application::get().getCamera();

    m_Shader->bind();
    m_Shader->setMatrix4("u_view", camera.viewMatrix());
    m_Shader->setMatrix4("u_projection", camera.projectionMatrix());
    m_Shader->unbind();
}

void MeshBody::hide() {
    auto &coordinator = gameLayer().getCoordinator();

    coordinator.SetComponentActive<Render3DComponent>(m_MeshBody, false);
    coordinator.SetComponentActive<ParentComponent>(m_MeshBody, false);

    BaseView::hide();
}

} // namespace Engine
