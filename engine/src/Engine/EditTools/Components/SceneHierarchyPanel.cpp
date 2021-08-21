#include "SceneHierarchyPanel.hpp"

#include "imgui/imgui.h"

#include "CameraComponent.hpp"
#include "SkeletComponent.hpp"
#include "TagComponent.hpp"
#include "VelocityComponent.hpp"

namespace Engine {

SceneHierarchyPanel::SceneHierarchyPanel(GameObjectModel &model) : m_Model(model) {}

void SceneHierarchyPanel::onDraw(int x, int y) {
    auto &gameCordinator = gameLayer().getCoordinator();

    ImGui::Begin("Scene Hierarchy");

    auto entities = gameCordinator.GetEntities();
    for (auto entity : entities) {
        auto &name = gameCordinator.GetEntityName(entity);
        auto &tag = gameCordinator.GetComponent<TagComponent>(entity).tag;

        ImGuiTreeNodeFlags flags = (m_Model.entity() == entity) ? ImGuiTreeNodeFlags_Selected : 0;
        flags |= ImGuiTreeNodeFlags_OpenOnArrow;
        flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

        bool opened = ImGui::TreeNodeEx((void *)(uint64_t)(uint32_t)entity, flags, (name + tag).c_str());

        if (ImGui::IsItemClicked()) {
            m_Model.setEntity(entity);
        }

        if (ImGui::BeginPopupContextItem()) {
            if (!gameCordinator.HasComponent<VelocityComponent>(entity) && ImGui::MenuItem("Add Velocity")) {
                gameCordinator.AddComponent<VelocityComponent>(entity, VelocityComponent());
            }

            if (!gameCordinator.HasComponent<SkeletComponent>(entity) && ImGui::MenuItem("Add Skelet Animation")) {
                gameCordinator.AddComponent<SkeletComponent>(entity, SkeletComponent());
            }

            if (!gameCordinator.HasComponent<CameraComponent>(entity) && ImGui::MenuItem("Bind Camera")) {
                auto cameraArray = gameCordinator.GetComponentArray<CameraComponent>();
                if (cameraArray->size() > 0) {
                    Entity entity = cameraArray->entities()[0];
                    gameCordinator.RemoveComponent<CameraComponent>(entity);
                }
                gameCordinator.AddComponent<CameraComponent>(entity, CameraComponent());
            }

            if (ImGui::MenuItem("Delete Entity")) {
                gameCordinator.DestroyEntity(entity);
                if (m_Model.entity() == entity)
                    m_Model.setEntity(c_NoEntity);
            }

            ImGui::EndPopup();
        }

        // if (opened) {
        //     ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
        //     bool opened = ImGui::TreeNodeEx((void *)9817239, flags, name.c_str());
        //     if (opened)
        //         ImGui::TreePop();
        //     ImGui::TreePop();
        // }
    }

    ImGui::End();
}

} // namespace Engine
