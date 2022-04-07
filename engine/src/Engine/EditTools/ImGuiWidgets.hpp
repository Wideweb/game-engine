#pragma once

#include "imgui/imgui.h"
#include <string>

namespace Engine::ImGuiWidgets {
static void Collapse(std::string label, bool &expanded) {
    std::string collapeLabel = (expanded ? "-  " : "+ ") + label;
    ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, {0, .5f});
    if (ImGui::Button(collapeLabel.c_str(), {ImGui::GetContentRegionAvailWidth(), 20.0f})) {
        expanded = !expanded;
    }
    ImGui::PopStyleVar();
}

static void PaddingLeft(float value) { ImGui::SetCursorPosX(ImGui::GetCursorPosX() + value); }

static void PaddingTop(float value) { ImGui::SetCursorPosY(ImGui::GetCursorPosY() + value); }
} // namespace Engine::ImGuiWidgets