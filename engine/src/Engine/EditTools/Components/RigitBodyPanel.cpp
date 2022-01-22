#include "RigitBodyPanel.hpp"

#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>

namespace Engine {

RigitBodyPanel::RigitBodyPanel(GameObjectModel &model) : m_Model(model) {}

void RigitBodyPanel::onAttach() {
    m_Model.weightChange$.addEventCallback([this](float weight) { m_Weight = weight; });
}

void RigitBodyPanel::onUpdate() {
    if (m_Weight != m_Model.weight()) {
        m_Model.weight(m_Weight);
    }
}

void RigitBodyPanel::onDraw(int x, int y) {
    ImGui::Begin("Rigitbody");

    ImGui::InputFloat("Mass", &m_Weight, 0.1f, 0.01f);

    ImGui::End();
}

} // namespace Engine
