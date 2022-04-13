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
    ImGuiWidgets::ComponentPanel<SkeletComponent>("Skelet", expanded, m_Model.entity(), gameLayer().getCoordinator(),
                                                  true);
    if (!expanded) {
        return;
    }

    ImGui::PushItemWidth(120.0f);
    float padding = 10.0f;

    ImGuiWidgets::PaddingLeft(padding);
    if (ImGui::BeginCombo("animation", m_Model.animation().c_str())) {

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
