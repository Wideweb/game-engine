#include "ParticlesPanel.hpp"

#include "ImGuiWidgets.hpp"
#include "ParticlesComponent.hpp"

#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>

namespace Engine {

ParticlesPanel::ParticlesPanel(GameObjectModel &model) : m_Model(model) {}

void ParticlesPanel::onDraw() {
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
    if (!ImGuiWidgets::ComponentPanel<ParticlesComponent>("Particles", expanded, m_Model.entity(),
                                                          gameLayer().getCoordinator(), true)) {
        return;
    }

    ParticlesConfiguration prevParticles = m_Model.particles();
    ParticlesConfiguration particles = prevParticles;

    float padding = 10.0f;
    ImGui::PushItemWidth(120.0f);
    {
        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputInt("Count", &particles.count, 1, 10);

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("Rate", &particles.rate, 0.001f, 0.01f);

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("Lifetime", &particles.lifeTime, 0.1f, 0.01f);

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("Radius", &particles.spawnRadius, 0.1f, 0.01f);

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("Velocity Min", &particles.velocityMin, 0.1f, 0.01f);

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("Velocity Max", &particles.velocityMax, 0.1f, 0.01f);

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("Polar", &particles.velocityPolarAngle, 0.01f, 0.01f);

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("Azimuth", &particles.velocityAzimuthalAngle, 0.01f, 0.01f);

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("Size From", &particles.sizeFrom, 0.1f, 0.01f);

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("Size To", &particles.sizeTo, 0.1f, 0.01f);

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::ColorEdit3("Color From", glm::value_ptr(particles.colorFrom));

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::ColorEdit3("Color To", glm::value_ptr(particles.colorTo));

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat3("Gravity", glm::value_ptr(particles.gravity));
    }

    ImGui::PopItemWidth();
    ImGui::NewLine();

    if (particles != prevParticles) {
        m_Model.particles(particles);
    }
}

} // namespace Engine
