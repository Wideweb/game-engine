#include "SceneHierarchyPanel.hpp"

#include "imgui/imgui.h"

#include "BehaviourComponent.hpp"
#include "CameraComponent.hpp"
#include "CollisionComponent.hpp"
#include "EditToolComponent.hpp"
#include "FlatDictionary.hpp"
#include "ParticlesComponent.hpp"
#include "PhysicsComponent.hpp"
#include "SkeletComponent.hpp"
#include "SpotLightComponent.hpp"
#include "TagComponent.hpp"
#include "TerrainCollisionComponent.hpp"
#include "Text2DComponent.hpp"
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

        if (!coordinator.HasComponent<Render3DComponent>(entity) && ImGui::MenuItem("Add 3D Model")) {
            coordinator.AddComponent<Render3DComponent>(entity, Render3DComponent());
        }

        if (!coordinator.HasComponent<SkeletComponent>(entity) && ImGui::MenuItem("Add Skelet Animation")) {
            coordinator.AddComponent<SkeletComponent>(entity, SkeletComponent());
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
            coordinator.AddComponent<TagComponent>(entity, TagComponent("platform"));
        }

        if (!coordinator.HasComponent<ParticlesComponent>(entity) && ImGui::MenuItem("Add Particles")) {
            coordinator.AddComponent<ParticlesComponent>(entity, ParticlesComponent());
        }

        if (!coordinator.HasComponent<BehaviourComponent>(entity) && ImGui::MenuItem("Add Behaviour")) {
            coordinator.AddComponent<BehaviourComponent>(entity, BehaviourComponent());
        }

        if (!coordinator.HasComponent<Text2DComponent>(entity) && ImGui::MenuItem("Add 2D Text")) {
            coordinator.AddComponent<Text2DComponent>(entity, Text2DComponent());
        }

        if (!coordinator.HasComponent<CameraComponent>(entity) && ImGui::MenuItem("Bind Camera")) {
            auto cmpArray = coordinator.GetComponentArray<CameraComponent>();
            CameraComponent cmp;
            if (cmpArray->size() > 0) {
                Entity entity = cmpArray->entities()[0];
                cmp = coordinator.GetComponent<CameraComponent>(entity);
                coordinator.RemoveComponent<CameraComponent>(entity);
            }
            coordinator.AddComponent<CameraComponent>(entity, CameraComponent());
        }

        if (!coordinator.HasComponent<DirectedLightComponent>(entity) && ImGui::MenuItem("Bind Directed Light")) {
            auto cmpArray = coordinator.GetComponentArray<DirectedLightComponent>();
            DirectedLightComponent cmp;
            if (cmpArray->size() > 0) {
                Entity entity = cmpArray->entities()[0];
                cmp = coordinator.GetComponent<DirectedLightComponent>(entity);
                coordinator.RemoveComponent<DirectedLightComponent>(entity);
            }
            coordinator.AddComponent<DirectedLightComponent>(entity, cmp);
        }

        if (!coordinator.HasComponent<SpotLightComponent>(entity) && ImGui::MenuItem("Add Spot Light")) {
            coordinator.AddComponent<SpotLightComponent>(entity, SpotLightComponent());
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

void SceneHierarchyPanel::onDraw() {
    auto &gameCordinator = gameLayer().getCoordinator();

    ImGui::Begin("Scene Hierarchy");

    auto entities = gameCordinator.GetEntities();

    SceneHierarchyNode hierarchy;
    std::unordered_map<Entity, SceneHierarchyNode> entityToNode;
    for (auto entity : entities) {
        if (gameCordinator.HasComponent<EditToolComponent>(entity) &&
            !gameCordinator.GetComponent<EditToolComponent>(entity).canSelect) {
            continue;
        }

        auto &node = entityToNode[entity];
        node.entity = entity;
        node.title = gameCordinator.GetEntityName(entity);
    }

    for (auto entity : entities) {
        if (gameCordinator.HasComponent<EditToolComponent>(entity) &&
            !gameCordinator.GetComponent<EditToolComponent>(entity).canSelect) {
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
