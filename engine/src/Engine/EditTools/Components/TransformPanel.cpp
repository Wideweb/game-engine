#include "TransformPanel.hpp"

#include "imgui/imgui.h"

namespace Engine {

TransformPanel::TransformPanel(GameObjectModel &model) : m_Model(model) {}

void TransformPanel::onAttach() {
    m_Model.positionChange$.addEventCallback([this](glm::vec3 position) { m_Position = position; });
    m_Model.rotationChange$.addEventCallback([this](glm::vec3 rotation) { m_Rotation = rotation; });
    m_Model.localRotationChange$.addEventCallback([this](glm::vec3 rotation) { m_LocalRotation = rotation; });
    m_Model.scaleChange$.addEventCallback([this](float scale, float prev) { m_Scale = scale; });
}

void TransformPanel::onUpdate() {
    if (m_Position != m_Model.position()) {
        m_Model.position(m_Position);
    }

    if (m_Rotation != m_Model.rotation()) {
        m_Model.rotation(m_Rotation);
    }

    if (m_LocalRotation != m_Model.localRotation()) {
        m_Model.localRotation(m_LocalRotation);
    }

    if (m_Scale != m_Model.scale()) {
        m_Model.scale(m_Scale);
    }
}

void TransformPanel::onDraw(int x, int y) {
    // ImGui::SetNextWindowSize(ImVec2(250, 200));
    // ImGui::SetNextWindowPos(ImVec2(x, y));

    ImGui::Begin("Transform");

    {
        ImGui::Text("Location: ");
        ImGui::InputFloat("x", &m_Position.x, 0.1f, 0.01f);
        ImGui::InputFloat("y", &m_Position.y, 0.1f, 0.01f);
        ImGui::InputFloat("z", &m_Position.z, 0.1f, 0.01f);
    }

    {
        ImGui::Text("Rotation: ");
        ImGui::InputFloat("x1", &m_Rotation.x, 0.1f, 0.01f);
        ImGui::InputFloat("y1", &m_Rotation.y, 0.1f, 0.01f);
        ImGui::InputFloat("z1", &m_Rotation.z, 0.1f, 0.01f);
    }

    {
        ImGui::Text("Local Rotation: ");
        ImGui::InputFloat("lx", &m_LocalRotation.x, 0.1f, 0.01f);
        ImGui::InputFloat("ly", &m_LocalRotation.y, 0.1f, 0.01f);
        ImGui::InputFloat("lz", &m_LocalRotation.z, 0.1f, 0.01f);
    }

    {
        ImGui::Text("Scale: ");
        ImGui::InputFloat("xyz", &m_Scale, 0.1f, 0.01f);
    }

    // ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::End();
}

} // namespace Engine
