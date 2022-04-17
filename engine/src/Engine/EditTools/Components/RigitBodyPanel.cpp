#include "RigitBodyPanel.hpp"

#include "ImGuiWidgets.hpp"
#include "PhysicsComponent.hpp"

#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>

namespace Engine {

RigitBodyPanel::RigitBodyPanel(GameObjectModel &model) : m_Model(model) {}

void RigitBodyPanel::onDraw() {
    static bool expanded = false;
    if (!ImGuiWidgets::ComponentPanel<PhysicsComponent>("Rigitbody", expanded, m_Model.entity(),
                                                        gameLayer().getCoordinator(), true)) {
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
