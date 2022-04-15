#include "SkeletPanel.hpp"

#include "ImGuiWidgets.hpp"
#include "SkeletComponent.hpp"

#include "Application.hpp"
#include "SkinnedModel.hpp"

#include "imgui/imgui.h"

namespace Engine {

SkeletPanel::SkeletPanel(GameObjectModel &model) : m_Model(model) {}

void SkeletPanel::onAttach() {}

void SkeletPanel::onUpdate() {}

void SkeletPanel::onDraw(int x, int y) {
    static bool expanded = false;
    if (!ImGuiWidgets::ComponentPanel<SkeletComponent>("Skelet", expanded, m_Model.entity(),
                                                       gameLayer().getCoordinator(), true)) {
        return;
    }

    float padding = 10.0f;

    ImGuiWidgets::PaddingLeft(padding);
    ImGui::Text("Animation");

    ImGuiWidgets::PaddingLeft(padding);
    ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() - padding);
    if (ImGui::BeginCombo("##SkeletAnimation", m_Model.animation().c_str())) {
        if (ImGui::Selectable("", m_Model.animation().empty())) {
            m_Model.animation("");
        }

        const auto &model = Application::get().getModels().GetModel<SkinnedModel>(m_Model.model());
        for (auto &[key, value] : model->skelet.animations) {
            bool isSelected = key == m_Model.animation();
            if (ImGui::Selectable(key.c_str(), isSelected)) {
                m_Model.animation(key);
            }
        }

        ImGui::EndCombo();
    }

    ImGui::PopItemWidth();
    ImGui::NewLine();
}

} // namespace Engine
