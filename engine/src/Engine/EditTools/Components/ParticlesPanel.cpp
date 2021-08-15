#include "ParticlesPanel.hpp"

#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>

namespace Engine {

ParticlesPanel::ParticlesPanel(GameObjectModel &model) : m_Model(model) {}

void ParticlesPanel::onAttach() {
    m_Model.particlesChange$.addEventCallback([this](ParticlesConfiguration particles) { m_Particles = particles; });
}

void ParticlesPanel::onUpdate() {
    if (m_Particles != m_Model.particles()) {
        m_Model.particles(m_Particles);
    }
}

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

    ImGui::Begin("Particles");

    {
        ImGui::InputInt("count", &m_Particles.count, 1, 10);
        ImGui::InputFloat("rate", &m_Particles.rate, 0.001f, 0.01f);
        ImGui::InputFloat("lifetime", &m_Particles.lifeTime, 0.1f, 0.01f);
        ImGui::InputFloat("radius", &m_Particles.spawnRadius, 0.1f, 0.01f);
        ImGui::InputFloat("vel min", &m_Particles.velocityMin, 0.1f, 0.01f);
        ImGui::InputFloat("vel max", &m_Particles.velocityMax, 0.1f, 0.01f);
        ImGui::InputFloat("polar", &m_Particles.velocityPolarAngle, 0.01f, 0.01f);
        ImGui::InputFloat("azimuth", &m_Particles.velocityAzimuthalAngle, 0.01f, 0.01f);
        ImGui::InputFloat("size from", &m_Particles.sizeFrom, 0.1f, 0.01f);
        ImGui::InputFloat("size to", &m_Particles.sizeTo, 0.1f, 0.01f);
        ImGui::ColorEdit3("color from", glm::value_ptr(m_Particles.colorFrom));
        ImGui::ColorEdit3("color to", glm::value_ptr(m_Particles.colorTo));
        ImGui::InputFloat3("gravity", glm::value_ptr(m_Particles.gravity));
    }

    ImGui::End();
}

} // namespace Engine
