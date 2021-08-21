#include "VelocityPanel.hpp"

#include "imgui/imgui.h"

namespace Engine {

VelocityPanel::VelocityPanel(GameObjectModel &model) : m_Model(model) {}

void VelocityPanel::onAttach() {
    m_Model.velocityChange$.addEventCallback([this](glm::vec3 velocity) { m_Velocity = velocity; });
    m_Model.rotationVelocityChange$.addEventCallback([this](glm::vec3 rotation) { m_Rotation = rotation; });
    m_Model.directionalSpeedChange$.addEventCallback([this](float speed) { m_Speed = speed; });
}

void VelocityPanel::onUpdate() {
    if (m_Velocity != m_Model.velocity()) {
        m_Model.velocity(m_Velocity);
    }

    if (m_Rotation != m_Model.rotationVelocity()) {
        m_Model.rotationVelocity(m_Rotation);
    }

    if (m_Speed != m_Model.directionalSpeed()) {
        m_Model.directionalSpeed(m_Speed);
    }
}

void VelocityPanel::onDraw(int x, int y) {
    ImGui::Begin("Velocity");

    {
        ImGui::Text("Velocity: ");
        ImGui::InputFloat("vx", &m_Velocity.x, 0.1f, 0.01f);
        ImGui::InputFloat("vy", &m_Velocity.y, 0.1f, 0.01f);
        ImGui::InputFloat("vz", &m_Velocity.z, 0.1f, 0.01f);
    }

    {
        ImGui::Text("Rotation: ");
        ImGui::InputFloat("rx", &m_Rotation.x, 0.1f, 0.01f);
        ImGui::InputFloat("ry", &m_Rotation.y, 0.1f, 0.01f);
        ImGui::InputFloat("rz", &m_Rotation.z, 0.1f, 0.01f);
    }

    {
        ImGui::Text("Directional speed: ");
        ImGui::InputFloat("value", &m_Speed, 0.1f, 0.01f);
    }

    ImGui::End();
}

} // namespace Engine
