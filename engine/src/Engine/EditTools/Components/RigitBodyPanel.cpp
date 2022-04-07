#include "RigitBodyPanel.hpp"

#include "ImGuiWidgets.hpp"

#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>

namespace Engine {

RigitBodyPanel::RigitBodyPanel(GameObjectModel &model) : m_Model(model) {}

void RigitBodyPanel::onDraw(int x, int y) {
    static bool expanded = false;
    ImGuiWidgets::Collapse("Rigitbody", expanded);
    if (!expanded) {
        return;
    }

    ImGui::PushItemWidth(120.0f);
    float padding = 10.0f;

    float prevWeight = m_Model.weight();
    float weight = prevWeight;
    ImGuiWidgets::PaddingLeft(padding);
    ImGui::InputFloat("Mass", &weight, 0.1f, 0.01f);
    if (prevWeight != weight) {
        m_Model.weight(weight);
    }

    ImGui::PopItemWidth();
    ImGui::NewLine();
}

} // namespace Engine
