#include "ParticlesPanel.hpp"

#include "ImGuiWidgets.hpp"

#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>

namespace Engine {

ParticlesPanel::ParticlesPanel(GameObjectModel &model) : m_Model(model) {}

void ParticlesPanel::onDraw(int x, int y) {
    // ImGui::SetNextWindowSize(ImVec2(250, 200));
    // ImGui::SetNextWindowPos(ImVec2(x, y));

    // glm::vec3 colorFrom = glm::vec3(0.0f, 0.0f, 0.0f);
    // glm::vec3 colorTo = glm::vec3(1.0f, 1.0f, 1.0f);
    // glm::vec3 gravity = glm::vec3(0.0, -0.5, 0.0);
    // size_t count = 1000;
    // float rate = 0.002f;
    // float sizeFrom = 100.0f;
    // float sizeTo = 0.0f;
    // float lifeTime = 1.0f;
    // float spawnRadius = 0.0f;
    // float velocityMin = 1.0f;
    // float velocityMax = 2.0f;
    // float velocityPolarAngle = 1.57f;
    // float velocityAzimuthalAngle = 6.28f;
    // bool looped = true;

    static bool expanded = false;
    ImGuiWidgets::Collapse("Particles", expanded);
    if (!expanded) {
        return;
    }

    ParticlesConfiguration prevParticles = m_Model.particles();
    ParticlesConfiguration particles = prevParticles;

    float padding = 10.0f;
    ImGui::PushItemWidth(120.0f);
    {
        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputInt("count", &particles.count, 1, 10);

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("rate", &particles.rate, 0.001f, 0.01f);

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("lifetime", &particles.lifeTime, 0.1f, 0.01f);

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("radius", &particles.spawnRadius, 0.1f, 0.01f);

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("vel min", &particles.velocityMin, 0.1f, 0.01f);

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("vel max", &particles.velocityMax, 0.1f, 0.01f);

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("polar", &particles.velocityPolarAngle, 0.01f, 0.01f);

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("azimuth", &particles.velocityAzimuthalAngle, 0.01f, 0.01f);

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("size from", &particles.sizeFrom, 0.1f, 0.01f);

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("size to", &particles.sizeTo, 0.1f, 0.01f);

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::ColorEdit3("color from", glm::value_ptr(particles.colorFrom));

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::ColorEdit3("color to", glm::value_ptr(particles.colorTo));

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat3("gravity", glm::value_ptr(particles.gravity));
    }

    ImGui::PopItemWidth();
    ImGui::NewLine();

    if (particles != prevParticles) {
        m_Model.particles(particles);
    }
}

} // namespace Engine
