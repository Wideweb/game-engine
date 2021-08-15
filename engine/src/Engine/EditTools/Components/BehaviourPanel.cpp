#include "BehaviourPanel.hpp"

#include "BehaviourComponent.hpp"

#include "imgui/imgui.h"
#include <filesystem>
#include <memory>

namespace Engine {

BehaviourPanel::BehaviourPanel(GameObjectModel &model) : m_Model(model) {}

void BehaviourPanel::onDraw(int x, int y) {
    ImGui::Begin("Script");

    ImGui::Button("script", ImVec2(100.0f, 0.0f));
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
            const char *path = static_cast<const char *>(payload->Data);
            std::filesystem::path script = std::filesystem::path("assets") / path;
            m_Model.behaviour(script);
        }
        ImGui::EndDragDropTarget();
    }

    ImGui::End();
}

} // namespace Engine
