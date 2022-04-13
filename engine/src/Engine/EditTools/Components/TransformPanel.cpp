#include "TransformPanel.hpp"

#include "ImGuiWidgets.hpp"
#include "LocationComponent.hpp"

#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>

#include <iostream>

namespace Engine {

TransformPanel::TransformPanel(GameObjectModel &model) : m_Model(model) {}

void TransformPanel::onDraw(int x, int y) {
    static bool expanded = false;
    ImGuiWidgets::ComponentPanel<LocationComponent>("Transform", expanded, m_Model.entity(),
                                                    gameLayer().getCoordinator());
    if (!expanded) {
        return;
    }

    auto currentOrientation =
        m_Model.transformOrientation() == GameObjectModel::TransformOrientation::Local ? "Local" : "Global";

    float padding = 10.0f;

    ImGuiWidgets::PaddingLeft(padding);
    ImGui::Text("Orientation");
    ImGui::PushItemWidth(120.0f);
    ImGuiWidgets::PaddingLeft(padding);
    if (ImGui::BeginCombo("##orientation", currentOrientation)) {
        if (ImGui::Selectable("Global",
                              m_Model.transformOrientation() == GameObjectModel::TransformOrientation::Global)) {
            m_Model.transformOrientation(GameObjectModel::TransformOrientation::Global);
        }

        if (ImGui::Selectable("Local",
                              m_Model.transformOrientation() == GameObjectModel::TransformOrientation::Local)) {
            m_Model.transformOrientation(GameObjectModel::TransformOrientation::Local);
        }

        ImGui::EndCombo();
    }

    {
        glm::vec3 prevPosition = m_Model.localPosition();
        glm::vec3 position = prevPosition;

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::Text("Location: ");

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("##locationX", &position.x, 0.1f, 0.01f);
        ImGui::SameLine();
        ImGui::Text("X");

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("##locationY", &position.y, 0.1f, 0.01f);
        ImGui::SameLine();
        ImGui::Text("Y");

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("##locationZ", &position.z, 0.1f, 0.01f);
        ImGui::SameLine();
        ImGui::Text("Z");

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
        ImGui::InputFloat("##rotationX", &rotation.x, 0.1f, 0.01f);
        ImGui::SameLine();
        ImGui::Text("X");

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("##rotationY", &rotation.y, 0.1f, 0.01f);
        ImGui::SameLine();
        ImGui::Text("Y");

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("##rotationZ", &rotation.z, 0.1f, 0.01f);
        ImGui::SameLine();
        ImGui::Text("Z");

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
        ImGui::InputFloat("##scaleX", &scale.x, 0.1f, 0.01f);
        ImGui::SameLine();
        ImGui::Text("X");

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("##scaleY", &scale.y, 0.1f, 0.01f);
        ImGui::SameLine();
        ImGui::Text("Y");

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("##scaleZ", &scale.z, 0.1f, 0.01f);
        ImGui::SameLine();
        ImGui::Text("Z");

        if (scale != prevScale) {
            m_Model.scale(scale);
        }
    }

    ImGui::PopItemWidth();
    ImGui::NewLine();

    // ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

} // namespace Engine
