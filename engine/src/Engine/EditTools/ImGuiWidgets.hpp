#pragma once

#include "imgui/imgui.h"
#include <string>

namespace Engine::ImGuiWidgets {

static void PaddingLeft(float value) { ImGui::SetCursorPosX(ImGui::GetCursorPosX() + value); }

static void PaddingTop(float value) { ImGui::SetCursorPosY(ImGui::GetCursorPosY() + value); }

static void Collapse(std::string label, bool &expanded) {
    std::string collapeLabel = (expanded ? "-  " : "+ ") + label;
    ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, {0, .5f});
    ImGuiWidgets::PaddingLeft(2.0f);
    if (ImGui::Button(collapeLabel.c_str(), {ImGui::GetContentRegionAvailWidth() - 2.0f, 20.0f})) {
        expanded = !expanded;
    }
    ImGui::PopStyleVar();
}

static void ToggledCollapse(std::string label, bool &expanded, bool &toggle) {
    std::string id = (std::string("##") + label);

    ImGuiWidgets::PaddingLeft(2.0f);
    ImGui::Checkbox(id.c_str(), &toggle);
    ImGui::SameLine();
    std::string collapeLabel = (expanded ? "-  " : "+ ") + label;
    ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, {0, .5f});
    ImGuiWidgets::PaddingLeft(-5.0f);
    if (ImGui::Button(collapeLabel.c_str(), {ImGui::GetContentRegionAvailWidth() - 2.0f, 20.0f})) {
        expanded = !expanded;
    }
    ImGui::PopStyleVar();
}

} // namespace Engine::ImGuiWidgets