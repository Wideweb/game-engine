#include "ConsolePanel.hpp"

#include "Application.hpp"

#include "imgui/imgui.h"

namespace Engine {

ConsolePanel::ConsolePanel() {}

void ConsolePanel::onAttach() {
    auto &eventHandler = Application::get().getEventHandler();
    eventHandler.add<ApplicationError>([this](const ApplicationError &e) {
        m_Logs += e.what();
        m_Logs += "\n";
    });
}

void ConsolePanel::onDraw() {
    ImGui::Begin("Console");
    const char *logs = m_Logs.data();
    ImGui::Text(logs);
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);
    ImGui::End();
}

} // namespace Engine
