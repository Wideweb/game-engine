#pragma once

#include "Coordinator.hpp"

#include "imgui/imgui.h"
#include <string>

namespace Engine::ImGuiWidgets {

static void PaddingLeft(float value) { ImGui::SetCursorPosX(ImGui::GetCursorPosX() + value); }

static void PaddingTop(float value) { ImGui::SetCursorPosY(ImGui::GetCursorPosY() + value); }

template <typename TComponent>
static bool ComponentPanel(std::string label, bool &expanded, Entity entity, Coordinator &coordinator,
                           bool canBeToggled = false) {
    bool prevToggle = coordinator.IsComponentActive<TComponent>(entity);
    bool toggle = prevToggle;
    bool removed = false;

    std::string id = (std::string("##") + label);

    ImGuiWidgets::PaddingLeft(2.0f);

    if (canBeToggled) {
        ImGui::Checkbox(id.c_str(), &toggle);
        ImGui::SameLine();
        ImGuiWidgets::PaddingLeft(-5.0f);
    }
    std::string collapeLabel = (expanded ? "-  " : "+ ") + label;
    ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, {0, .5f});
    if (ImGui::Button(collapeLabel.c_str(),
                      {ImGui::GetContentRegionAvailWidth() - (canBeToggled ? 25.0f : 2.0f), 20.0f})) {
        expanded = !expanded;
    }
    ImGui::PopStyleVar();

    if (canBeToggled) {
        ImGui::SameLine();
        ImGuiWidgets::PaddingLeft(-5.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, {.5f, .5f});
        std::string deleteButtonId = id + "x";
        ImGui::PushID(deleteButtonId.c_str());
        if (ImGui::Button("x", {20.0f, 20.0f})) {
            coordinator.RemoveComponent<TComponent>(entity);
            removed = true;
        }
        ImGui::PopID();
        ImGui::PopStyleVar();
    }

    if (prevToggle != toggle) {
        coordinator.SetComponentActive<TComponent>(entity, toggle);
    }

    return expanded && !removed;
}

static bool CollapsePanel(std::string label, bool &expanded, bool &toggle) {
    std::string id = (std::string("##") + label);

    ImGuiWidgets::PaddingLeft(2.0f);

    std::string toggleId = id + "_toggle";
    ImGui::PushID(toggleId.c_str());
    ImGui::Checkbox(id.c_str(), &toggle);
    ImGui::PopID();
    ImGui::SameLine();
    ImGuiWidgets::PaddingLeft(-5.0f);

    std::string collapeLabel = (expanded ? "-  " : "+ ") + label;
    ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, {0, .5f});
    std::string collapseId = id + "_collapse";
    ImGui::PushID(collapseId.c_str());
    if (ImGui::Button(collapeLabel.c_str(), {ImGui::GetContentRegionAvailWidth() - 2.0f, 20.0f})) {
        expanded = !expanded;
    }
    ImGui::PopID();
    ImGui::PopStyleVar();

    return expanded;
}

static bool CollapsePanel(std::string label, bool &expanded) {
    std::string id = (std::string("##") + label);

    ImGuiWidgets::PaddingLeft(2.0f);

    std::string collapeLabel = (expanded ? "-  " : "+ ") + label;
    ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, {0, .5f});
    std::string collapseId = id + "_collapse";
    ImGui::PushID(collapseId.c_str());
    if (ImGui::Button(collapeLabel.c_str(), {ImGui::GetContentRegionAvailWidth() - 2.0f, 20.0f})) {
        expanded = !expanded;
    }
    ImGui::PopID();
    ImGui::PopStyleVar();

    return expanded;
}

} // namespace Engine::ImGuiWidgets