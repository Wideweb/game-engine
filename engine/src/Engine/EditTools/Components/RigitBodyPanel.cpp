#include "RigitBodyPanel.hpp"

#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>

namespace Engine {

RigitBodyPanel::RigitBodyPanel(GameObjectModel &model) : m_Model(model) {}

void RigitBodyPanel::onDraw(int x, int y) {
    ImGui::Begin("Rigitbody");

    float prevWeight = m_Model.weight();
    float weight = prevWeight;
    ImGui::InputFloat("Mass", &weight, 0.1f, 0.01f);
    if (prevWeight != weight) {
        m_Model.weight(weight);
    }

    ImGui::End();
}

} // namespace Engine
