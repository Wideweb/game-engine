#include "DirectedLightPanel.hpp"

#include "DirectedLightComponent.hpp"
#include "ImGuiWidgets.hpp"

#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>

namespace Engine {

DirectedLightPanel::DirectedLightPanel(GameObjectModel &model) : m_Model(model) {}

void DirectedLightPanel::onDraw() {
    static bool expanded = false;
    if (!ImGuiWidgets::ComponentPanel<DirectedLightComponent>("Directed Light", expanded, m_Model.entity(),
                                                              gameLayer().getCoordinator(), true)) {
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
        ImGui::InputFloat("Near", &light.nearPlane, 0.1f, 0.01f);

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("Far", &light.farPlane, 0.1f, 0.01f);
    }

    {
        ImGuiWidgets::PaddingLeft(padding);
        ImGui::Text("Shadow: ");

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputDouble("Bias Factor", &light.biasFactor, 0.1f, 0.01f);

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputDouble("Bias Min", &light.biasMin, 0.1f, 0.01f);

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputInt("PCF", &light.pcf);
    }

    ImGui::PopItemWidth();
    ImGui::NewLine();

    if (light != prevLight) {
        m_Model.directedLight(light);
    }
}

} // namespace Engine
