#include "Text2DPanel.hpp"

#include "Configs.hpp"
#include "File.hpp"
#include "ImGuiWidgets.hpp"
#include "Text2DComponent.hpp"

#include "imgui/imgui.h"
#include <filesystem>
#include <glm/gtc/type_ptr.hpp>
#include <string>

namespace Engine {

Text2DPanel::Text2DPanel(GameObjectModel &model) : m_Model(model) {}

void Text2DPanel::onDraw() {
    auto &coordinator = gameLayer().getCoordinator();

    static bool expanded = false;
    if (!ImGuiWidgets::ComponentPanel<Text2DComponent>("Text 2D", expanded, m_Model.entity(),
                                                       gameLayer().getCoordinator(), true)) {
        return;
    }

    ImGui::PushItemWidth(120.0f);
    float padding = 10.0f;

    auto &text2D = coordinator.GetComponent<Text2DComponent>(m_Model.entity());

    {
        static char message[64];
        strcpy(message, text2D.text.c_str());

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::Text("Message: ");

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() - padding);
        ImGui::InputText("##Text2DPanel_message", message, 64);
        std::string newMessage(message);

        if (newMessage != text2D.text) {
            text2D.text = newMessage;
            text2D.updated = true;
        }
    }

    {
        ImGuiWidgets::PaddingLeft(padding);
        ImGui::Text("Font");

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() - padding);
        if (ImGui::BeginCombo("##Text2DPanel_font", text2D.font.c_str())) {
            if (ImGui::Selectable("", text2D.font.empty())) {
                text2D.font = "";
                text2D.updated = true;
            }
            const auto &fonts = Application::get().getFonts().Keys();
            for (const auto &font : fonts) {
                bool isSelected = font == text2D.font;
                if (ImGui::Selectable(font.c_str(), isSelected)) {
                    text2D.font = font;
                    text2D.updated = true;
                }
            }
            ImGui::EndCombo();
        }

        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
                const char *path = static_cast<const char *>(payload->Data);
                std::string name = File::name(path);
                if (File::isExtension(name, Configs::c_FontExtensions)) {
                    auto &fonts = Application::get().getFonts();
                    if (!fonts.Has(name)) {
                        fonts.Add(name, std::filesystem::path("assets") / path);
                    }

                    text2D.font = name;
                    text2D.updated = true;
                }
            }
            ImGui::EndDragDropTarget();
        }
    }

    ImGui::PushItemWidth(120.0f);

    {
        glm::vec3 prevColor = text2D.color;
        glm::vec3 color = prevColor;

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::ColorEdit3("Color", glm::value_ptr(color));

        if (color != prevColor) {
            text2D.color = color;
            text2D.updated = true;
        }
    }

    {
        std::string currentAlignment;
        switch (text2D.horizontalAlignment) {
        case Text2DComponent::HorizontalAlignment::Left:
            currentAlignment = "Left";
            break;
        case Text2DComponent::HorizontalAlignment::Middle:
            currentAlignment = "Middle";
            break;
        case Text2DComponent::HorizontalAlignment::Right:
            currentAlignment = "Right";
            break;
        }

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::Text("Horizontal Alignment");
        ImGui::PushItemWidth(120.0f);
        ImGuiWidgets::PaddingLeft(padding);
        if (ImGui::BeginCombo("##Text2DPanel_horizontalAlignment", currentAlignment.c_str())) {
            if (ImGui::Selectable("Left", text2D.horizontalAlignment == Text2DComponent::HorizontalAlignment::Left)) {
                text2D.horizontalAlignment = Text2DComponent::HorizontalAlignment::Left;
                text2D.updated = true;
            }

            if (ImGui::Selectable("Middle",
                                  text2D.horizontalAlignment == Text2DComponent::HorizontalAlignment::Middle)) {
                text2D.horizontalAlignment = Text2DComponent::HorizontalAlignment::Middle;
                text2D.updated = true;
            }

            if (ImGui::Selectable("Right", text2D.horizontalAlignment == Text2DComponent::HorizontalAlignment::Right)) {
                text2D.horizontalAlignment = Text2DComponent::HorizontalAlignment::Right;
                text2D.updated = true;
            }

            ImGui::EndCombo();
        }
    }

    {
        std::string currentAlignment;
        switch (text2D.verticalAlignment) {
        case Text2DComponent::VerticalAlignment::Bottom:
            currentAlignment = "Bottom";
            break;
        case Text2DComponent::VerticalAlignment::Middle:
            currentAlignment = "Middle";
            break;
        case Text2DComponent::VerticalAlignment::Top:
            currentAlignment = "Top";
            break;
        }

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::Text("Vertical Alignment");
        ImGui::PushItemWidth(120.0f);
        ImGuiWidgets::PaddingLeft(padding);
        if (ImGui::BeginCombo("##Text2DPanel_VerticalAlignment", currentAlignment.c_str())) {
            if (ImGui::Selectable("Top", text2D.verticalAlignment == Text2DComponent::VerticalAlignment::Top)) {
                text2D.verticalAlignment = Text2DComponent::VerticalAlignment::Top;
                text2D.updated = true;
            }

            if (ImGui::Selectable("Middle", text2D.verticalAlignment == Text2DComponent::VerticalAlignment::Middle)) {
                text2D.verticalAlignment = Text2DComponent::VerticalAlignment::Middle;
                text2D.updated = true;
            }

            if (ImGui::Selectable("Bottom", text2D.verticalAlignment == Text2DComponent::VerticalAlignment::Bottom)) {
                text2D.verticalAlignment = Text2DComponent::VerticalAlignment::Bottom;
                text2D.updated = true;
            }

            ImGui::EndCombo();
        }
    }

    ImGui::PopItemWidth();
    ImGui::NewLine();
}

} // namespace Engine
