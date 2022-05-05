#include "RenderPanel.hpp"

#include "Application.hpp"

#include "ImGuiWidgets.hpp"

#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>

namespace Engine {

void RenderPanel::onAttach() {
    m_SkyboxPanel = std::make_unique<SkyboxPanel>();
    m_SkyboxPanel->onAttach();
    m_SkyboxPanel->show();
}

void RenderPanel::onUpdate() {
    if (m_SkyboxPanel->isVisible()) {
        m_SkyboxPanel->onUpdate();
    }
}

void RenderPanel::onDraw() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Scene Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::PopStyleVar();

    ImGui::PushItemWidth(120.0f);

    ImGuiWidgets::PaddingTop(4.0f);

    onDrawGammaCorrectionPanel();
    onDrawHdrPanel();
    onDrawSSAOPanel();
    onDrawFogPanel();

    ImGuiWidgets::PaddingLeft(2.0f);
    ImGui::Checkbox("NormalMapping", &gameLayer().renderSettings.normalMapping);

    if (m_SkyboxPanel->isVisible()) {
        m_SkyboxPanel->onDraw();
    }

    ImGui::PopItemWidth();
    ImGui::End();
}

void RenderPanel::onDrawGammaCorrectionPanel() {
    static bool expanded = false;
    if (!ImGuiWidgets::CollapsePanel("Gamma Correction", expanded, gameLayer().renderSettings.gamma)) {
        return;
    }

    float padding = 10.0f;

    ImGuiWidgets::PaddingLeft(padding);
    ImGui::InputFloat("Gamma Value", &gameLayer().renderSettings.gammaValue, 0.1f);

    ImGui::NewLine();
}

void RenderPanel::onDrawHdrPanel() {
    static bool expanded = false;
    if (!ImGuiWidgets::CollapsePanel("HDR", expanded, gameLayer().renderSettings.hdr)) {
        return;
    }

    float padding = 10.0f;

    ImGuiWidgets::PaddingLeft(padding);
    if (ImGui::BeginCombo("Tone Mapping", getToneMappingName(gameLayer().renderSettings.toneMapping).c_str())) {

        for (auto toneMapping : c_ToneMappings) {
            if (ImGui::Selectable(getToneMappingName(toneMapping).c_str(),
                                  gameLayer().renderSettings.toneMapping == toneMapping)) {
                gameLayer().renderSettings.toneMapping = toneMapping;
            }
        }

        ImGui::EndCombo();
    }

    ImGuiWidgets::PaddingLeft(padding);
    ImGui::InputFloat("Exposure", &gameLayer().renderSettings.exposure, 0.1f, 0.01f);

    ImGuiWidgets::PaddingLeft(padding);
    ImGui::InputFloat("Threshold", &gameLayer().renderSettings.threshold, 0.1f, 0.01f);

    ImGuiWidgets::PaddingLeft(padding);
    ImGui::InputInt("Bloom Scale", &gameLayer().renderSettings.bloomScale);
    if (gameLayer().renderSettings.bloomScale < 1) {
        gameLayer().renderSettings.bloomScale = 1;
    }

    ImGuiWidgets::PaddingLeft(padding);
    ImGui::InputInt("Blur", &gameLayer().renderSettings.blur);

    ImGui::NewLine();
}

void RenderPanel::onDrawSSAOPanel() {
    static bool expanded = false;
    if (!ImGuiWidgets::CollapsePanel("SSAO", expanded, gameLayer().renderSettings.ssao)) {
        return;
    }

    float padding = 10.0f;

    ImGuiWidgets::PaddingLeft(padding);
    ImGui::InputInt("Noise Scale", &gameLayer().renderSettings.ssaoNoiseScale);

    ImGuiWidgets::PaddingLeft(padding);
    ImGui::InputInt("Kernel Size", &gameLayer().renderSettings.ssaoKernelSize);

    ImGuiWidgets::PaddingLeft(padding);
    ImGui::InputFloat("Radius", &gameLayer().renderSettings.ssaoRadius, 0.1f, 0.01f);

    ImGuiWidgets::PaddingLeft(padding);
    ImGui::InputFloat("Bias", &gameLayer().renderSettings.ssaoBias, 0.01f, 0.001f);

    ImGui::NewLine();
}

void RenderPanel::onDrawFogPanel() {
    static bool expanded = false;
    if (!ImGuiWidgets::CollapsePanel("Fog", expanded, gameLayer().renderSettings.fog)) {
        return;
    }

    float padding = 10.0f;

    ImGuiWidgets::PaddingLeft(padding);
    ImGui::PushID("#Render_Panel_Fog_Color");
    ImGui::ColorEdit3("Color", glm::value_ptr(gameLayer().renderSettings.fogColor));
    ImGui::PopID();

    ImGuiWidgets::PaddingLeft(padding);
    ImGui::PushID("#Render_Panel_Fog_Density");
    ImGui::InputFloat("Density", &gameLayer().renderSettings.fogDensity, 0.01f);
    ImGui::PopID();

    ImGuiWidgets::PaddingLeft(padding);
    ImGui::PushID("#Render_Panel_Fog_Gradient");
    ImGui::InputFloat("Gradient", &gameLayer().renderSettings.fogGradient, 0.1f);
    ImGui::PopID();

    ImGui::NewLine();
}

void RenderPanel::onDetach() { m_SkyboxPanel->onDetach(); }

} // namespace Engine
