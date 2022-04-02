#include "DirectedLightPanel.hpp"

#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>

namespace Engine {

DirectedLightPanel::DirectedLightPanel(GameObjectModel &model) : m_Model(model) {}

void DirectedLightPanel::onDraw(int x, int y) {
    DirectedLight prevLight = m_Model.directedLight();
    DirectedLight light = prevLight;

    ImGui::Begin("Directed Light");

    ImGui::ColorEdit3("Ambient", glm::value_ptr(light.ambient));
    ImGui::ColorEdit3("Diffuse", glm::value_ptr(light.diffuse));
    ImGui::ColorEdit3("Specular", glm::value_ptr(light.specular));

    ImGui::SliderFloat("Intensity", &light.intensity, 0.0f, 10.0f);

    {
        ImGui::Text("Plane: ");
        ImGui::InputFloat("near", &light.nearPlane, 0.1f, 0.01f);
        ImGui::InputFloat("far", &light.farPlane, 0.1f, 0.01f);
    }

    {
        ImGui::Text("Shadow: ");
        ImGui::InputDouble("bias factor", &light.biasFactor, 0.1f, 0.01f);
        ImGui::InputDouble("bias min", &light.biasMin, 0.1f, 0.01f);
        ImGui::InputInt("pcf", &light.pcf);
    }

    ImGui::End();

    if (light != prevLight) {
        m_Model.directedLight(light);
    }
}

} // namespace Engine
