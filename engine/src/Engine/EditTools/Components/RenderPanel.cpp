#include "RenderPanel.hpp"

#include "Application.hpp"

#include "imgui/imgui.h"

namespace Engine {

void RenderPanel::onAttach() {
    m_Exposure = gameLayer().renderSettings.exposure;
    m_Gamma = gameLayer().renderSettings.gamma;
    m_Threshold = gameLayer().renderSettings.threshold;
    m_BloomScale = gameLayer().renderSettings.bloomScale;
    m_Blur = gameLayer().renderSettings.blur;
}

void RenderPanel::onUpdate() {
    gameLayer().renderSettings.toneMapping = m_ToneMapping;
    gameLayer().renderSettings.exposure = m_Exposure;
    gameLayer().renderSettings.gamma = m_Gamma;
    gameLayer().renderSettings.threshold = m_Threshold;
    gameLayer().renderSettings.bloomScale = m_BloomScale;
    gameLayer().renderSettings.blur = m_Blur;
}

void RenderPanel::onDraw(int x, int y) {
    ImGui::Begin("Render");

    {
        ImGui::Text("Hdr: ");

        if (ImGui::BeginCombo("tone mapping", getToneMappingName(m_ToneMapping).c_str())) {

            for (auto toneMapping : c_ToneMappings) {
                if (ImGui::Selectable(getToneMappingName(toneMapping).c_str(), m_ToneMapping == toneMapping)) {
                    m_ToneMapping = toneMapping;
                }
            }

            ImGui::EndCombo();
        }

        ImGui::InputFloat("exposure", &m_Exposure, 0.1f, 0.01f);
        ImGui::InputFloat("gamma", &m_Gamma, 0.1f, 0.01f);
        ImGui::InputFloat("threshold", &m_Threshold, 0.1f, 0.01f);
        ImGui::InputInt("bloomScale", &m_BloomScale);
        ImGui::InputInt("blur", &m_Blur);
    }

    ImGui::End();
}

} // namespace Engine
