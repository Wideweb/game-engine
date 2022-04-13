#include "CollisionPanel.hpp"

#include "CollisionComponent.hpp"
#include "ImGuiWidgets.hpp"
#include "Math.hpp"
#include "StaticCollisionComponent.hpp"

#include "imgui/imgui.h"
#include <cmath>
#include <glm/gtc/type_ptr.hpp>

namespace Engine {

CollisionPanel::CollisionPanel(GameObjectModel &model) : m_Model(model) {}

void CollisionPanel::onAttach() {
    m_Model.entityChange$.addEventCallback([this](Entity entity) {
        m_Static = gameLayer().getCoordinator().HasComponent<StaticCollisionComponent>(entity);
    });

    m_Model.collisionChange$.addEventCallback([this](const std::vector<glm::vec3> &vertices) {
        float width = std::abs(vertices[0].x);
        float height = std::abs(vertices[0].y);
        float depth = std::abs(vertices[0].z);

        m_Size = glm::vec3(width, height, depth);
    });
}

void CollisionPanel::onUpdate() {
    auto &coordinator = gameLayer().getCoordinator();
    auto entity = m_Model.entity();

    if (m_Static && coordinator.HasComponent<CollisionComponent>(entity)) {
        auto &current = coordinator.GetComponent<CollisionComponent>(entity);
        auto next = StaticCollisionComponent();
        next.vertices = current.vertices;

        bool isActive = coordinator.IsComponentActive<CollisionComponent>(entity);

        coordinator.RemoveComponent<CollisionComponent>(entity);
        coordinator.AddComponent<StaticCollisionComponent>(entity, next);
        coordinator.SetComponentActive<StaticCollisionComponent>(entity, isActive);

    } else if (!m_Static && coordinator.HasComponent<StaticCollisionComponent>(entity)) {
        auto &current = coordinator.GetComponent<StaticCollisionComponent>(entity);
        auto next = CollisionComponent();
        next.vertices = current.vertices;

        bool isActive = coordinator.IsComponentActive<StaticCollisionComponent>(entity);

        coordinator.RemoveComponent<StaticCollisionComponent>(entity);
        coordinator.AddComponent<CollisionComponent>(entity, next);
        coordinator.SetComponentActive<CollisionComponent>(entity, isActive);
    }

    std::vector<glm::vec3> vertices = m_Model.collisionBox();

    float width = std::abs(vertices[0].x);
    float height = std::abs(vertices[0].y);
    float depth = std::abs(vertices[0].z);

    glm::vec3 size = glm::vec3(width, height, depth);

    if (size != m_Size) {
        for (size_t i = 0; i < vertices.size(); i++) {
            vertices[i] = Math::rescale(vertices[i], size, m_Size);
        }

        m_Model.collisionBox(vertices);
    }
}

void CollisionPanel::onDraw(int x, int y) {
    static bool expanded = false;
    if (m_Static) {
        ImGuiWidgets::ComponentPanel<StaticCollisionComponent>("Box Collider", expanded, m_Model.entity(),
                                                               gameLayer().getCoordinator(), true);
    } else {
        ImGuiWidgets::ComponentPanel<CollisionComponent>("Box Collider", expanded, m_Model.entity(),
                                                         gameLayer().getCoordinator(), true);
    }

    if (!expanded) {
        return;
    }

    ImGui::PushItemWidth(120.0f);

    float padding = 10.0f;

    ImGuiWidgets::PaddingLeft(padding);
    ImGui::Checkbox("Static", &m_Static);

    ImGuiWidgets::PaddingLeft(padding);
    ImGui::InputFloat("width", &m_Size.x, 0.1f, 0.01f);
    ImGuiWidgets::PaddingLeft(padding);
    ImGui::InputFloat("height", &m_Size.y, 0.1f, 0.01f);
    ImGuiWidgets::PaddingLeft(padding);
    ImGui::InputFloat("depth", &m_Size.z, 0.1f, 0.01f);

    ImGui::PopItemWidth();
    ImGui::NewLine();
}

} // namespace Engine
