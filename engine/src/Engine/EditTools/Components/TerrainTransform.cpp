#include "TerrainTransform.hpp"

#include "Application.hpp"
#include "EditToolComponent.hpp"
#include "File.hpp"
#include "Math.hpp"
#include "ModelFactory.hpp"
#include "ParentComponent.hpp"
#include "Render3DComponent.hpp"
#include "TerrainCollisionComponent.hpp"

#include "cmath"
#include "imgui/imgui.h"

namespace Engine {

TerrainTransform::TerrainTransform(GameObjectModel &model, TerrainPanel &terrainPanel)
    : m_Model(model), m_TerrainPanel(terrainPanel) {}

void TerrainTransform::onAttach() {
    auto vertexSrc = File::read("./shaders/brush-vertex-shader.glsl");
    auto fragmentSrc = File::read("./shaders/brush-fragment-shader.glsl");
    m_Shader = std::make_shared<Shader>(vertexSrc, fragmentSrc);

    auto &coordinator = gameLayer().getCoordinator();
    m_Brush = coordinator.CreateEntity("terrain-brush");
    coordinator.AddComponent(m_Brush, LocationComponent());
    coordinator.AddComponent(m_Brush, EditToolComponent());
}

void TerrainTransform::onUpdate() {
    const auto &camera = Application::get().getCamera();
    auto ray = Application::get().getMousePicker().ray();
    auto pos = Application::get().getCamera().positionVec();
    const auto &collision = gameLayer().getCollision();
    auto &coordinator = gameLayer().getCoordinator();

    auto result = collision.RaycastTerrain(m_Model.entity(), pos, ray, 100.0f);
    if (result) {
        auto &render = coordinator.GetComponent<Render3DComponent>(m_Model.entity());
        auto modelVertices = Application::get().getModels().GetModel<InstancedModel>(render.model)->meshes[0].vertices;

        const auto &collider = collision.GetShape(m_Model.entity());

        float transformStrength = m_TerrainPanel.brushStrength;
        float radiusSquare = m_TerrainPanel.brushRadius * m_TerrainPanel.brushRadius;

        glm::vec2 terrainOrigin = glm::vec2(collider.box.min.x, collider.box.min.z);
        glm::vec2 point = glm::vec2(result->x, result->z);

        m_MousePos = point + terrainOrigin;

        if (m_MouseDown) {
            for (int i = 0; i < collider.vertices.size(); i++) {
                const auto vertex = collider.vertices[i];
                glm::vec2 delta = glm::vec2(vertex.x, vertex.z) - terrainOrigin - point;
                float distanceSquare = glm::dot(delta, delta);
                if (distanceSquare < radiusSquare) {
                    float impact = 1.0f - distanceSquare / radiusSquare;
                    modelVertices[i].position.y += transformStrength * impact;
                }
            }
            m_Model.updateTerrain(modelVertices);

            m_Edit = true;
        }
    }
}

void TerrainTransform::onDraw(int x, int y) {
    const auto &camera = Application::get().getCamera();

    m_Shader->bind();
    m_Shader->setMatrix4("u_view", camera.viewMatrix());
    m_Shader->setMatrix4("u_projection", camera.projectionMatrix());
    m_Shader->setFloat("u_radius", m_TerrainPanel.brushRadius);
    m_Shader->setFloat2("u_mouse", m_MousePos);
    m_Shader->unbind();
}

void TerrainTransform::onMouseEvent(MouseEvent &event) {
    event.handled = m_Edit;

    if (event.type == EventType::MouseDown) {
        m_MouseDown = true;
    }

    if (event.type == EventType::MouseUp) {
        m_MouseDown = false;
        m_Edit = false;
        event.handled = false;
    }
}

void TerrainTransform::show() {
    auto &coordinator = gameLayer().getCoordinator();

    const auto terrainRender = coordinator.GetComponent<Render3DComponent>(m_Model.entity());
    if (coordinator.HasComponent<Render3DComponent>(m_Brush)) {
        auto &brushRender = coordinator.GetComponent<Render3DComponent>(m_Brush);
        brushRender.setModel(terrainRender.model);
    } else {
        auto brushRender = Render3DComponent(terrainRender.model, 1.0, true);
        brushRender.shader = m_Shader;
        coordinator.AddComponent(m_Brush, brushRender);
    }

    if (coordinator.HasComponent<ParentComponent>(m_Brush)) {
        coordinator.GetComponent<ParentComponent>(m_Brush).setEntity(m_Model.entity());
    } else {
        coordinator.AddComponent(m_Brush, ParentComponent(m_Model.entity()));
    }

    BaseView::show();
}

void TerrainTransform::hide() {
    auto &coordinator = gameLayer().getCoordinator();

    if (coordinator.HasComponent<ParentComponent>(m_Brush)) {
        coordinator.RemoveComponent<ParentComponent>(m_Brush);
    }
    if (coordinator.HasComponent<Render3DComponent>(m_Brush)) {
        coordinator.RemoveComponent<Render3DComponent>(m_Brush);
    }

    BaseView::hide();
}

} // namespace Engine
