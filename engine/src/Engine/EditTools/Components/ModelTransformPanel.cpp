#include "ModelTransformPanel.hpp"

#include "ImGuiWidgets.hpp"

#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>

#include <iostream>

namespace Engine {

ModelTransformPanel::ModelTransformPanel(GameObjectModel &model) : m_Model(model) {}

void ModelTransformPanel::onDraw(int x, int y) {
    static bool expanded = false;
    ImGuiWidgets::Collapse("Model Transform", expanded);
    if (!expanded) {
        return;
    }

    ImGui::PushItemWidth(120.0f);
    float padding = 10.0f;

    {
        glm::vec3 prevRotation = m_Model.renderRotation();
        glm::vec3 rotation = prevRotation;

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::Text("Rotation: ");

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("##renderRotationX", &rotation.x, 0.1f, 0.01f);
        ImGui::SameLine();
        ImGui::Text("X");

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("##renderRotationY", &rotation.y, 0.1f, 0.01f);
        ImGui::SameLine();
        ImGui::Text("Y");

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("##renderRotationZ", &rotation.z, 0.1f, 0.01f);
        ImGui::SameLine();
        ImGui::Text("Z");

        if (rotation != prevRotation) {
            m_Model.renderRotation(rotation);
        }
    }

    {
        glm::vec3 prevScale = m_Model.renderScale();
        glm::vec3 scale = prevScale;

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::Text("Scale: ");

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("##renderScaleX", &scale.x, 0.1f, 0.01f);
        ImGui::SameLine();
        ImGui::Text("X");

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("##renderScaleY", &scale.y, 0.1f, 0.01f);
        ImGui::SameLine();
        ImGui::Text("Y");

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("##renderScaleZ", &scale.z, 0.1f, 0.01f);
        ImGui::SameLine();
        ImGui::Text("Z");

        if (scale != prevScale) {
            m_Model.renderScale(scale);
        }
    }

    ImGui::PopItemWidth();
    ImGui::NewLine();

    // ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

} // namespace Engine
