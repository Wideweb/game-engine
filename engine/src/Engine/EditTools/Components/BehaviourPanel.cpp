#include "BehaviourPanel.hpp"

#include "BehaviourComponent.hpp"
#include "Configs.hpp"
#include "File.hpp"
#include "ImGuiWidgets.hpp"

#include "imgui/imgui.h"
#include <filesystem>
#include <memory>

namespace Engine {

BehaviourPanel::BehaviourPanel(GameObjectModel &model) : m_Model(model) {}

void BehaviourPanel::onDraw(int x, int y) {
    static bool expanded = false;
    ImGuiWidgets::Collapse("Behaviour", expanded);
    if (!expanded) {
        return;
    }

    ImGui::PushItemWidth(120.0f);
    float padding = 10.0f;

    ImGuiWidgets::PaddingLeft(padding);
    ImGui::Button("script", ImVec2(100.0f, 0.0f));
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
            const char *path = static_cast<const char *>(payload->Data);
            if (File::isExtension(path, Configs::c_ScriptExtensions)) {
                std::filesystem::path script = std::filesystem::path("assets") / path;
                m_Model.behaviour(script);
            }
        }
        ImGui::EndDragDropTarget();
    }

    ImGui::PopItemWidth();
    ImGui::NewLine();
}

} // namespace Engine
