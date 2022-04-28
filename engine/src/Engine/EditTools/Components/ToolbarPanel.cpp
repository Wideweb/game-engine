#include "ToolbarPanel.hpp"

#include "Application.hpp"
#include "TextureLoader.hpp"

#include "imgui/imgui.h"

namespace Engine {

ToolbarPanel::ToolbarPanel() {}

void ToolbarPanel::onAttach() {
    m_IconPlay = TextureLoader::loadTexture("assets/textures/icons/play.png");
    m_IconStop = TextureLoader::loadTexture("assets/textures/icons/stop.png");
}

void ToolbarPanel::onDraw() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    auto &colors = ImGui::GetStyle().Colors;
    const auto &buttonHovered = colors[ImGuiCol_ButtonHovered];
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
    const auto &buttonActive = colors[ImGuiCol_ButtonActive];
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

    ImGui::Begin("##toolbar", nullptr,
                 ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    float size = ImGui::GetWindowHeight() - 10.0f;
    Texture &icon = Application::get().getTime().poused() ? m_IconPlay : m_IconStop;
    ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
    ImGui::SetCursorPosY((ImGui::GetWindowContentRegionMax().y * 0.5f) - (size * 0.5f));

    if (ImGui::ImageButton((ImTextureID)icon.id, ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0)) {
        if (Application::get().getTime().poused()) {
            Application::get().getTime().play();
            m_CameraPos = Application::get().getCamera().positionVec();
            m_CameraRotation = Application::get().getCamera().rotationQuat();
        } else {
            Application::get().getTime().stop();
            Application::get().getCamera().setPosition(m_CameraPos);
            Application::get().getCamera().setRotation(m_CameraRotation);
        }
    }

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(3);
    ImGui::End();
}

void ToolbarPanel::onDetach() {
    m_IconPlay.free();
    m_IconStop.free();
}

} // namespace Engine
