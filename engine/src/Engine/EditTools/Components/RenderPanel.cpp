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

    m_SSAO = gameLayer().renderSettings.ssao;
    m_SSAONoiseScale = gameLayer().renderSettings.ssaoNoiseScale;
    m_SSAOKernelSize = gameLayer().renderSettings.ssaoKernelSize;
    m_SSAORadius = gameLayer().renderSettings.ssaoRadius;
    m_SSAOBias = gameLayer().renderSettings.ssaoBias;

    m_NormalMapping = gameLayer().renderSettings.normalMapping;
}

void RenderPanel::onUpdate() {
    gameLayer().renderSettings.toneMapping = m_ToneMapping;
    gameLayer().renderSettings.exposure = m_Exposure;
    gameLayer().renderSettings.gamma = m_Gamma;
    gameLayer().renderSettings.threshold = m_Threshold;
    gameLayer().renderSettings.bloomScale = m_BloomScale;
    gameLayer().renderSettings.blur = m_Blur;

    gameLayer().renderSettings.ssao = m_SSAO;
    gameLayer().renderSettings.ssaoNoiseScale = m_SSAONoiseScale;
    gameLayer().renderSettings.ssaoKernelSize = m_SSAOKernelSize;
    gameLayer().renderSettings.ssaoRadius = m_SSAORadius;
    gameLayer().renderSettings.ssaoBias = m_SSAOBias;

    gameLayer().renderSettings.normalMapping = m_NormalMapping;
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

    {
        ImGui::Text("SSAO: ");

        ImGui::Checkbox("enable", &m_SSAO);
        ImGui::InputInt("noiseScale", &m_SSAONoiseScale);
        ImGui::InputInt("kernelSize", &m_SSAOKernelSize);
        ImGui::InputFloat("radius", &m_SSAORadius, 0.1f, 0.01f);
        ImGui::InputFloat("bias", &m_SSAOBias, 0.01f, 0.001f);
    }

    { ImGui::Checkbox("NormalMapping", &m_NormalMapping); }

    ImGui::End();
}

} // namespace Engine
