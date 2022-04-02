#include "VelocityPanel.hpp"

#include "imgui/imgui.h"
#include <glm/vec3.hpp>

namespace Engine {

VelocityPanel::VelocityPanel(GameObjectModel &model) : m_Model(model) {}

void VelocityPanel::onDraw(int x, int y) {
    ImGui::Begin("Velocity");

    {
        glm::vec3 prevVelocity = m_Model.velocity();
        glm::vec3 velocity = prevVelocity;

        ImGui::Text("Velocity: ");
        ImGui::InputFloat("vx", &velocity.x, 0.1f, 0.01f);
        ImGui::InputFloat("vy", &velocity.y, 0.1f, 0.01f);
        ImGui::InputFloat("vz", &velocity.z, 0.1f, 0.01f);

        if (prevVelocity != velocity) {
            m_Model.velocity(velocity);
        }
    }

    {
        glm::vec3 prevRotation = m_Model.rotationVelocity();
        glm::vec3 rotation = prevRotation;

        ImGui::Text("Rotation: ");
        ImGui::InputFloat("rx", &rotation.x, 0.1f, 0.01f);
        ImGui::InputFloat("ry", &rotation.y, 0.1f, 0.01f);
        ImGui::InputFloat("rz", &rotation.z, 0.1f, 0.01f);

        if (prevRotation != rotation) {
            m_Model.rotationVelocity(rotation);
        }
    }

    {
        float prevSpeed = m_Model.directionalSpeed();
        float speed = prevSpeed;

        ImGui::Text("Directional speed: ");
        ImGui::InputFloat("value", &speed, 0.1f, 0.01f);

        if (prevSpeed != speed) {
            m_Model.directionalSpeed(speed);
        }
    }

    ImGui::End();
}

} // namespace Engine
