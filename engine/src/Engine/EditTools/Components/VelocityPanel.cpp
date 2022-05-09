#include "VelocityPanel.hpp"

#include "ImGuiWidgets.hpp"
#include "VelocityComponent.hpp"

#include "imgui/imgui.h"
#include <glm/vec3.hpp>

namespace Engine {

VelocityPanel::VelocityPanel(GameObjectModel &model) : m_Model(model) {}

void VelocityPanel::onDraw() {
    static bool expanded = false;
    if (!ImGuiWidgets::ComponentPanel<VelocityComponent>("Velocity", expanded, m_Model.entity(),
                                                         gameLayer().getCoordinator(), true)) {
        return;
    }

    ImGui::PushItemWidth(120.0f);
    float padding = 10.0f;

    {
        glm::vec3 prevVelocity = m_Model.velocity();
        glm::vec3 velocity = prevVelocity;

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::Text("Velocity: ");

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("##velocity_x", &velocity.x, 0.1f, 0.01f);
        ImGui::SameLine();
        ImGui::Text("X");

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("##velocity_y", &velocity.y, 0.1f, 0.01f);
        ImGui::SameLine();
        ImGui::Text("Y");

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("##velocity_z", &velocity.z, 0.1f, 0.01f);
        ImGui::SameLine();
        ImGui::Text("Z");

        if (prevVelocity != velocity) {
            m_Model.velocity(velocity);
        }
    }

    {
        glm::vec3 prevRotation = m_Model.rotationVelocity();
        glm::vec3 rotation = prevRotation;

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::Text("Rotation: ");

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("##rotation_x", &rotation.x, 0.1f, 0.01f);
        ImGui::SameLine();
        ImGui::Text("X");

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("##rotation_y", &rotation.y, 0.1f, 0.01f);
        ImGui::SameLine();
        ImGui::Text("Y");

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("##rotation_z", &rotation.z, 0.1f, 0.01f);
        ImGui::SameLine();
        ImGui::Text("Z");

        if (prevRotation != rotation) {
            m_Model.rotationVelocity(rotation);
        }
    }

    {
        float prevSpeed = m_Model.directionalSpeed();
        float speed = prevSpeed;

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::Text("Directional Speed: ");
        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("##Directional_Speed", &speed, 0.1f, 0.01f);

        if (prevSpeed != speed) {
            m_Model.directionalSpeed(speed);
        }
    }

    ImGui::PopItemWidth();
    ImGui::NewLine();
}

} // namespace Engine
