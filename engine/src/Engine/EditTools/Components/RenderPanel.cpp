#include "RenderPanel.hpp"

#include "Application.hpp"

#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>

namespace Engine {

void RenderPanel::onDraw(int x, int y) {
    ImGui::Begin("Render");

    {
        ImGui::Checkbox("Hdr", &gameLayer().renderSettings.hdr);

        if (gameLayer().renderSettings.hdr) {

            if (ImGui::BeginCombo("tone mapping", getToneMappingName(gameLayer().renderSettings.toneMapping).c_str())) {

                for (auto toneMapping : c_ToneMappings) {
                    if (ImGui::Selectable(getToneMappingName(toneMapping).c_str(),
                                          gameLayer().renderSettings.toneMapping == toneMapping)) {
                        gameLayer().renderSettings.toneMapping = toneMapping;
                    }
                }

                ImGui::EndCombo();
            }

            ImGui::InputFloat("exposure", &gameLayer().renderSettings.exposure, 0.1f, 0.01f);
            ImGui::InputFloat("gamma", &gameLayer().renderSettings.gamma, 0.1f, 0.01f);
            ImGui::InputFloat("threshold", &gameLayer().renderSettings.threshold, 0.1f, 0.01f);
            ImGui::InputInt("bloomScale", &gameLayer().renderSettings.bloomScale);
            if (gameLayer().renderSettings.bloomScale < 1) {
                gameLayer().renderSettings.bloomScale = 1;
            }
            ImGui::InputInt("blur", &gameLayer().renderSettings.blur);
        }
    }

    {
        ImGui::Checkbox("SSAO", &gameLayer().renderSettings.ssao);

        if (gameLayer().renderSettings.ssao) {
            ImGui::InputInt("noiseScale", &gameLayer().renderSettings.ssaoNoiseScale);
            ImGui::InputInt("kernelSize", &gameLayer().renderSettings.ssaoKernelSize);
            ImGui::InputFloat("radius", &gameLayer().renderSettings.ssaoRadius, 0.1f, 0.01f);
            ImGui::InputFloat("bias", &gameLayer().renderSettings.ssaoBias, 0.01f, 0.001f);
        }
    }

    { ImGui::Checkbox("NormalMapping", &gameLayer().renderSettings.normalMapping); }

    ImGui::End();
}

} // namespace Engine
