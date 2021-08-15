#include "DirectedLightPanel.hpp"

#include "imgui/imgui.h"

namespace Engine {

DirectedLightPanel::DirectedLightPanel(DirectedLightModel &model) : m_Model(model) {}

void DirectedLightPanel::onAttach() {
    m_Light = m_Model.light();
    m_Model.change$.addEventCallback([this](DirectedLight light) { m_Light = light; });
}

void DirectedLightPanel::onUpdate() {
    if (m_Light != m_Model.light()) {
        m_Model.light(m_Light);
    }
}

void DirectedLightPanel::onDraw(int x, int y) {
    // ImGui::SetNextWindowSize(ImVec2(150, 450));
    // ImGui::SetNextWindowPos(ImVec2(x, y));

    ImGui::Begin("Directed Light");

    {
        ImGui::Text("Ambient: ");
        ImGui::InputFloat("x", &m_Light.ambient.x, 0.1f, 0.01f);
        ImGui::InputFloat("y", &m_Light.ambient.y, 0.1f, 0.01f);
        ImGui::InputFloat("z", &m_Light.ambient.z, 0.1f, 0.01f);
    }

    {
        ImGui::Text("Diffuse: ");
        ImGui::InputFloat("x1", &m_Light.diffuse.x, 0.1f, 0.01f);
        ImGui::InputFloat("y1", &m_Light.diffuse.y, 0.1f, 0.01f);
        ImGui::InputFloat("z1", &m_Light.diffuse.z, 0.1f, 0.01f);
    }

    {
        ImGui::Text("Specular: ");
        ImGui::InputFloat("x2", &m_Light.specular.x, 0.1f, 0.01f);
        ImGui::InputFloat("y2", &m_Light.specular.y, 0.1f, 0.01f);
        ImGui::InputFloat("z2", &m_Light.specular.z, 0.1f, 0.01f);
    }

    {
        ImGui::Text("Direction: ");
        ImGui::InputFloat("x3", &m_Light.direction.x, 0.1f, 0.01f);
        ImGui::InputFloat("y3", &m_Light.direction.y, 0.1f, 0.01f);
        ImGui::InputFloat("z3", &m_Light.direction.z, 0.1f, 0.01f);
    }

    {
        ImGui::Text("Plane: ");
        ImGui::InputFloat("near", &m_Light.nearPlane, 0.1f, 0.01f);
        ImGui::InputFloat("far", &m_Light.farPlane, 0.1f, 0.01f);
    }

    ImGui::End();
}

} // namespace Engine
