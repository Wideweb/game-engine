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

void BehaviourPanel::onDraw() {
    static bool expanded = false;
    if (!ImGuiWidgets::ComponentPanel<BehaviourComponent>("Behaviour", expanded, m_Model.entity(),
                                                          gameLayer().getCoordinator(), true)) {
        return;
    }

    ImGui::PushItemWidth(120.0f);
    float padding = 10.0f;

    std::string id = "##BehaviourPanel";

    ImGuiWidgets::PaddingLeft(padding);
    ImGui::Text("Script");

    bool hasScript = !m_Model.behaviour().script.empty();

    ImGuiWidgets::PaddingLeft(padding);
    ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, {0, .5f});
    ImGui::Button(m_Model.behaviour().script.c_str(),
                  {ImGui::GetContentRegionAvailWidth() - padding - (hasScript ? 25.0f : 0.0f), 20.0f});
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
    ImGui::PopStyleVar();

    if (hasScript) {
        ImGui::SameLine();
        ImGuiWidgets::PaddingLeft(-5.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, {.5f, .5f});
        std::string deleteButtonId = id + "x";
        ImGui::PushID(deleteButtonId.c_str());
        if (ImGui::Button("x", {20.0f, 20.0f})) {
            m_Model.behaviour("");
            gameLayer().getScripts().remove(m_Model.entity());
        }
        ImGui::PopID();
        ImGui::PopStyleVar();
    }

    ImGui::PopItemWidth();
    ImGui::NewLine();
}

} // namespace Engine
