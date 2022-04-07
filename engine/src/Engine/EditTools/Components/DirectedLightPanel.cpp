#include "DirectedLightPanel.hpp"

#include "ImGuiWidgets.hpp"

#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>

namespace Engine {

DirectedLightPanel::DirectedLightPanel(GameObjectModel &model) : m_Model(model) {}

void DirectedLightPanel::onDraw(int x, int y) {
    static bool expanded = false;
    ImGuiWidgets::Collapse("Directed Light", expanded);
    if (!expanded) {
        return;
    }

    ImGui::PushItemWidth(120.0f);
    float padding = 10.0f;

    DirectedLight prevLight = m_Model.directedLight();
    DirectedLight light = prevLight;

    ImGuiWidgets::PaddingLeft(padding);
    ImGui::ColorEdit3("Ambient", glm::value_ptr(light.ambient));

    ImGuiWidgets::PaddingLeft(padding);
    ImGui::ColorEdit3("Diffuse", glm::value_ptr(light.diffuse));

    ImGuiWidgets::PaddingLeft(padding);
    ImGui::ColorEdit3("Specular", glm::value_ptr(light.specular));

    ImGuiWidgets::PaddingLeft(padding);
    ImGui::SliderFloat("Intensity", &light.intensity, 0.0f, 10.0f);

    {
        ImGuiWidgets::PaddingLeft(padding);
        ImGui::Text("Plane: ");

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("near", &light.nearPlane, 0.1f, 0.01f);

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("far", &light.farPlane, 0.1f, 0.01f);
    }

    {
        ImGuiWidgets::PaddingLeft(padding);
        ImGui::Text("Shadow: ");

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputDouble("bias factor", &light.biasFactor, 0.1f, 0.01f);

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputDouble("bias min", &light.biasMin, 0.1f, 0.01f);

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputInt("pcf", &light.pcf);
    }

    ImGui::PopItemWidth();
    ImGui::NewLine();

    if (light != prevLight) {
        m_Model.directedLight(light);
    }
}

} // namespace Engine
