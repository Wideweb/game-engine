#include "TransformPanel.hpp"

#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>

#include <iostream>

namespace Engine {

TransformPanel::TransformPanel(GameObjectModel &model) : m_Model(model) {}

void TransformPanel::onDraw(int x, int y) {
    ImGui::Begin("Transform");

    auto currentOrientation =
        m_Model.transformOrientation() == GameObjectModel::TransformOrientation::Local ? "Local" : "Global";

    if (ImGui::BeginCombo("orientation", currentOrientation)) {
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

        ImGui::Text("Location: ");
        ImGui::InputFloat("x", &position.x, 0.1f, 0.01f);
        ImGui::InputFloat("y", &position.y, 0.1f, 0.01f);
        ImGui::InputFloat("z", &position.z, 0.1f, 0.01f);

        if (position != prevPosition) {
            m_Model.localPosition(position);
        }
    }

    {
        glm::vec3 prevRotation = m_Model.localRotation();
        glm::vec3 rotation = prevRotation;

        ImGui::Text("Rotation: ");
        ImGui::InputFloat("x1", &rotation.x, 0.1f, 0.01f);
        ImGui::InputFloat("y1", &rotation.y, 0.1f, 0.01f);
        ImGui::InputFloat("z1", &rotation.z, 0.1f, 0.01f);

        if (rotation != prevRotation) {
            m_Model.localRotation(rotation);
        }
    }

    {
        glm::vec3 prevRotation = m_Model.renderRotation();
        glm::vec3 rotation = prevRotation;

        ImGui::Text("Render Rotation: ");
        ImGui::InputFloat("lx", &rotation.x, 0.1f, 0.01f);
        ImGui::InputFloat("ly", &rotation.y, 0.1f, 0.01f);
        ImGui::InputFloat("lz", &rotation.z, 0.1f, 0.01f);

        if (rotation != prevRotation) {
            m_Model.renderRotation(rotation);
        }
    }

    {
        glm::vec3 prevScale = m_Model.scale();
        glm::vec3 scale = prevScale;

        ImGui::Text("Scale: ");
        ImGui::InputFloat3("xyz", glm::value_ptr(scale));

        if (scale != prevScale) {
            m_Model.scale(scale);
        }
    }

    // ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::End();
}

} // namespace Engine
