#include "SpotLightPanel.hpp"

#include "ImGuiWidgets.hpp"
#include "SpotLightComponent.hpp"

#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>

namespace Engine {

SpotLightPanel::SpotLightPanel(GameObjectModel &model) : m_Model(model) {}

void SpotLightPanel::onDraw() {
    auto &coordinator = gameLayer().getCoordinator();

    static bool expanded = false;
    if (!ImGuiWidgets::ComponentPanel<SpotLightComponent>("Spot Light", expanded, m_Model.entity(), coordinator,
                                                          true)) {
        return;
    }

    ImGui::PushItemWidth(120.0f);
    float padding = 10.0f;

    auto &spotLightCmp = coordinator.GetComponent<SpotLightComponent>(m_Model.entity());

    SpotLight prevLight = spotLightCmp.light;
    SpotLight light = prevLight;

    {
        ImGuiWidgets::PaddingLeft(padding);
        ImGui::Text("Light: ");

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::ColorEdit3("Ambient", glm::value_ptr(light.ambient));

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::ColorEdit3("Diffuse", glm::value_ptr(light.diffuse));

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::ColorEdit3("Specular", glm::value_ptr(light.specular));

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::SliderFloat("##Spot_Light-Intencity", &light.intensity, 0.0f, 10.0f);
        ImGui::SameLine();
        ImGui::Text("Intensity");
    }

    {
        ImGuiWidgets::PaddingLeft(padding);
        ImGui::Text("Attenuation: ");

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("##Spot_Light-Constant", &light.constant, 0.1f);
        ImGui::SameLine();
        ImGui::Text("Constant");

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("##Spot_Light-Linear", &light.linear, 0.01f);
        ImGui::SameLine();
        ImGui::Text("Linear");

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("##Spot_Light-Quadratic", &light.quadratic, 0.001f);
        ImGui::SameLine();
        ImGui::Text("Quadratic");
    }

    {
        ImGuiWidgets::PaddingLeft(padding);
        ImGui::Text("Shadow Frustum: ");

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputInt("##Spot_Light-Width", &light.shadowFrustumWidth, 1.0f);
        ImGui::SameLine();
        ImGui::Text("Width");

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputInt("##Spot_Light-Height", &light.shadowFrustumHeight, 1.0f);
        ImGui::SameLine();
        ImGui::Text("Height");

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("##Spot_Light-NearPlane", &light.shadowFrustumNearPlane, 0.1f);
        ImGui::SameLine();
        ImGui::Text("Near Plane");

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("##Spot_Light-FarPlane", &light.shadowFrustumFarPlane, 0.1f);
        ImGui::SameLine();
        ImGui::Text("Far Plane");
    }

    {
        ImGuiWidgets::PaddingLeft(padding);
        ImGui::Text("Shadow Settings: ");

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputDouble("##Spot_Light-Bias_Min", &light.bias, 0.001f);
        ImGui::SameLine();
        ImGui::Text("Bias");

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputInt("##Spot_Light-PCF_Samples", &light.pcfSamples);
        light.pcfSamples = light.pcfSamples < 0 ? 0 : light.pcfSamples;
        light.pcfSamples = light.pcfSamples > 20 ? 20 : light.pcfSamples;
        ImGui::SameLine();
        ImGui::Text("PCF Samples");

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputDouble("##Spot_Light-PCF_Disk_Radius", &light.pcfDiskRadius, 0.001f);
        ImGui::SameLine();
        ImGui::Text("PCF Disk Radius");
    }

    ImGui::PopItemWidth();
    ImGui::NewLine();

    if (light != prevLight) {
        spotLightCmp.light = light;
    }
}

} // namespace Engine
