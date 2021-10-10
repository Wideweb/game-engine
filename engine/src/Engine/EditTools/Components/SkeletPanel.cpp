#include "SkeletPanel.hpp"

#include "Application.hpp"
#include "SkinnedModel.hpp"

#include "imgui/imgui.h"

namespace Engine {

SkeletPanel::SkeletPanel(GameObjectModel &model) : m_Model(model) {}

void SkeletPanel::onAttach() {}

void SkeletPanel::onUpdate() {}

void SkeletPanel::onDraw(int x, int y) {
    ImGui::Begin("Skelet");

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

    ImGui::End();
}

} // namespace Engine
