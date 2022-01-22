#include "SceneHierarchyPanel.hpp"

#include "imgui/imgui.h"

#include "CameraComponent.hpp"
#include "CollisionComponent.hpp"
#include "EditToolComponent.hpp"
#include "FlatDictionary.hpp"
#include "ParticlesComponent.hpp"
#include "PhysicsComponent.hpp"
#include "TerrainCollisionComponent.hpp"
#include "VelocityComponent.hpp"

#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

namespace Engine {

void SceneHierarchyNode::addTools(GameObjectModel &model, Coordinator &coordinator) const {
    if (ImGui::IsItemClicked()) {
        model.setEntity(entity);
    }

    if (ImGui::BeginDragDropSource()) {
        const char *itemName = title.c_str();
        ImGui::SetDragDropPayload("SCENE_HIERARCHY_ITEM", title.c_str(), (strlen(itemName) + 1) * sizeof(char));
        ImGui::EndDragDropSource();
    }

    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("SCENE_HIERARCHY_ITEM")) {
            const char *droppedEntityName = static_cast<const char *>(payload->Data);
            Entity droppedEntity = coordinator.GetEntity(droppedEntityName);

            glm::mat4 prevTransform = model.transform();
            model.parent(entity);
            glm::mat4 nextParentTransform = model.parentTransform();

            glm::mat4 entityTransform = glm::inverse(nextParentTransform) * prevTransform;

            glm::vec3 scale;
            glm::quat rotation;
            glm::vec3 position;
            glm::vec3 skew;
            glm::vec4 perspective;
            glm::decompose(entityTransform, scale, rotation, position, skew, perspective);

            model.localPosition(position);
            model.localRotation(glm::eulerAngles(rotation));
            model.localScale(scale);
        }
        ImGui::EndDragDropTarget();
    }

    if (ImGui::BeginPopupContextItem()) {
        if (!coordinator.HasComponent<VelocityComponent>(entity) && ImGui::MenuItem("Add Velocity")) {
            coordinator.AddComponent<VelocityComponent>(entity, VelocityComponent());
        }

        if (!coordinator.HasComponent<PhysicsComponent>(entity) && ImGui::MenuItem("Add Rigitbody")) {
            coordinator.AddComponent<PhysicsComponent>(entity, PhysicsComponent());
        }

        if (!coordinator.HasComponent<CollisionComponent>(entity) &&
            !coordinator.HasComponent<StaticCollisionComponent>(entity) && ImGui::MenuItem("Add Box Collision")) {
            coordinator.AddComponent<CollisionComponent>(entity, CollisionComponent(glm::vec3(1.0f)));
        }

        if (!coordinator.HasComponent<TerrainCollisionComponent>(entity) && ImGui::MenuItem("Add Terrain Collision")) {
            coordinator.AddComponent<TerrainCollisionComponent>(entity, TerrainCollisionComponent());
        }

        if (!coordinator.HasComponent<ParticlesComponent>(entity) && ImGui::MenuItem("Add Particles")) {
            coordinator.AddComponent<ParticlesComponent>(entity, ParticlesComponent());
        }

        if (!coordinator.HasComponent<CameraComponent>(entity) && ImGui::MenuItem("Bind Camera")) {
            auto cameraArray = coordinator.GetComponentArray<CameraComponent>();
            if (cameraArray->size() > 0) {
                Entity entity = cameraArray->entities()[0];
                coordinator.RemoveComponent<CameraComponent>(entity);
            }
            coordinator.AddComponent<CameraComponent>(entity, CameraComponent());
        }

        if (ImGui::MenuItem("Delete Entity")) {
            coordinator.DestroyEntity(entity);
            if (model.entity() == entity)
                model.setEntity(c_NoEntity);
        }

        ImGui::EndPopup();
    }
}

void SceneHierarchyNode::display(GameObjectModel &model, Coordinator &coordinator, bool childOnly) const {
    if (childOnly) {
        for (const auto &child : children) {
            child->display(model, coordinator);
        }
        return;
    }

    ImGuiTreeNodeFlags flags = (model.entity() == entity) ? ImGuiTreeNodeFlags_Selected : 0;
    flags |= ImGuiTreeNodeFlags_SpanFullWidth;

    if (children.size() > 0) {
        bool open = ImGui::TreeNodeEx(title.c_str(), flags);
        addTools(model, coordinator);

        if (open) {
            for (const auto &child : children) {
                child->display(model, coordinator);
            }
            ImGui::TreePop();
        }
    } else {
        flags |= ImGuiTreeNodeFlags_Leaf;
        flags |= ImGuiTreeNodeFlags_Bullet;
        flags |= ImGuiTreeNodeFlags_NoTreePushOnOpen;

        ImGui::TreeNodeEx(title.c_str(), flags);
        addTools(model, coordinator);
    }
}

SceneHierarchyPanel::SceneHierarchyPanel(GameObjectModel &model) : m_Model(model) {}

void SceneHierarchyPanel::onDraw(int x, int y) {
    auto &gameCordinator = gameLayer().getCoordinator();

    ImGui::Begin("Scene Hierarchy");

    auto entities = gameCordinator.GetEntities();

    SceneHierarchyNode hierarchy;
    std::unordered_map<Entity, SceneHierarchyNode> entityToNode;
    for (auto entity : entities) {
        auto &node = entityToNode[entity];
        node.entity = entity;
        node.title = gameCordinator.GetEntityName(entity);
    }

    for (auto entity : entities) {
        if (gameCordinator.HasComponent<EditToolComponent>(entity)) {
            continue;
        }

        if (!gameCordinator.HasComponent<ParentComponent>(entity)) {
            hierarchy.children.push_back(&entityToNode[entity]);
            continue;
        }

        Entity parent = gameCordinator.GetComponent<ParentComponent>(entity).entity;
        entityToNode[parent].children.push_back(&entityToNode[entity]);
    }

    hierarchy.display(m_Model, gameCordinator, true);

    ImGui::End();
}

} // namespace Engine
