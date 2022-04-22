#include "Transform2DPanel.hpp"

#include "ImGuiWidgets.hpp"
#include "Location2DComponent.hpp"

#include "imgui/imgui.h"
#include <glm/vec3.hpp>

namespace Engine {

Transform2DPanel::Transform2DPanel(GameObjectModel &model) : m_Model(model) {}

void Transform2DPanel::onDraw() {
    auto &coordinator = gameLayer().getCoordinator();
    auto &location2d = coordinator.GetComponent<Location2DComponent>(m_Model.entity());

    static bool expanded = false;
    if (!ImGuiWidgets::ComponentPanel<Location2DComponent>("Transform 2D", expanded, m_Model.entity(), coordinator)) {
        return;
    }

    ImGui::PushItemWidth(120.0f);
    float padding = 10.0f;

    ImGuiWidgets::PaddingLeft(padding);
    ImGui::Text("Location: ");

    {
        bool prevNdc = location2d.ndc;
        bool ndc = prevNdc;

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::Checkbox("NDC", &ndc);

        if (ndc != prevNdc) {
            location2d.ndc = ndc;
            location2d.updated = true;
        }
    }

    {
        glm::vec3 prevPosition = m_Model.localPosition();
        glm::vec3 position = prevPosition;

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("##Transform2DPanel_locationX", &position.x, location2d.ndc ? 0.1 : 1.0f);
        ImGui::SameLine();
        ImGui::Text("X");

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("##Transform2DPanel_locationY", &position.y, location2d.ndc ? 0.1 : 1.0f);
        ImGui::SameLine();
        ImGui::Text("Y");

        if (position != prevPosition) {
            m_Model.localPosition(position);
        }
    }

    {
        glm::vec3 prevRotation = m_Model.localRotation();
        glm::vec3 rotation = prevRotation;

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::Text("Rotation: ");

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("##Transform2DPanel_rotation", &rotation.z, 0.01f);
        ImGui::SameLine();
        ImGui::Text("Radians");

        if (rotation != prevRotation) {
            m_Model.localRotation(rotation);
        }
    }

    {
        glm::vec3 prevScale = m_Model.scale();
        glm::vec3 scale = prevScale;

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::Text("Scale: ");

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("##Transform2DPanel_scaleX", &scale.x, 0.1f);
        ImGui::SameLine();
        ImGui::Text("X");

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("##Transform2DPanel_scaleY", &scale.y, 0.1f);
        ImGui::SameLine();
        ImGui::Text("Y");

        if (scale != prevScale) {
            m_Model.scale(scale);
        }
    }

    ImGui::PopItemWidth();
    ImGui::NewLine();
}

} // namespace Engine
