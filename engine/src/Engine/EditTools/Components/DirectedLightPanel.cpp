#include "DirectedLightPanel.hpp"

#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>

namespace Engine {

DirectedLightPanel::DirectedLightPanel(GameObjectModel &model) : m_Model(model) {}

void DirectedLightPanel::onAttach() {
    m_Light = m_Model.directedLight();
    m_Model.directedLightChange$.addEventCallback([this](DirectedLight light) { m_Light = light; });
}

void DirectedLightPanel::onUpdate() {
    if (m_Light != m_Model.directedLight()) {
        m_Model.directedLight(m_Light);
    }
}

void DirectedLightPanel::onDraw(int x, int y) {
    ImGui::Begin("Directed Light");

    ImGui::ColorEdit3("Ambient", glm::value_ptr(m_Light.ambient));
    ImGui::ColorEdit3("Diffuse", glm::value_ptr(m_Light.diffuse));
    ImGui::ColorEdit3("Specular", glm::value_ptr(m_Light.specular));

    ImGui::SliderFloat("Intensity", &m_Light.intensity, 0.0f, 10.0f);

    {
        ImGui::Text("Plane: ");
        ImGui::InputFloat("near", &m_Light.nearPlane, 0.1f, 0.01f);
        ImGui::InputFloat("far", &m_Light.farPlane, 0.1f, 0.01f);
    }

    {
        ImGui::Text("Shadow: ");
        ImGui::InputDouble("bias factor", &m_Light.biasFactor, 0.1f, 0.01f);
        ImGui::InputDouble("bias min", &m_Light.biasMin, 0.1f, 0.01f);
        ImGui::InputInt("pcf", &m_Light.pcf);
    }

    ImGui::End();
}

} // namespace Engine
