#include "TransformPanel.hpp"

#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>

namespace Engine {

TransformPanel::TransformPanel(GameObjectModel &model) : m_Model(model) {}

void TransformPanel::onAttach() {
    m_Model.positionChange$.addEventCallback([this](glm::vec3 position) { m_Position = position; });
    m_Model.rotationChange$.addEventCallback([this](glm::vec3 rotation) { m_Rotation = rotation; });
    m_Model.renderRotationChange$.addEventCallback([this](glm::vec3 rotation) { m_RenderRotation = rotation; });
    m_Model.scaleChange$.addEventCallback([this](glm::vec3 scale, glm::vec3 prev) { m_Scale = scale; });
}

void TransformPanel::onUpdate() {
    if (m_Position != m_Model.localPosition()) {
        m_Model.localPosition(m_Position);
    }

    if (m_Rotation != m_Model.localRotation()) {
        m_Model.localRotation(m_Rotation);
    }

    if (m_RenderRotation != m_Model.renderRotation()) {
        m_Model.renderRotation(m_RenderRotation);
    }

    if (m_Scale != m_Model.scale()) {
        m_Model.scale(m_Scale);
    }
}

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
        ImGui::Text("Location: ");
        ImGui::InputFloat("x", &m_Position.x, 0.1f, 0.01f);
        ImGui::InputFloat("y", &m_Position.y, 0.1f, 0.01f);
        ImGui::InputFloat("z", &m_Position.z, 0.1f, 0.01f);
    }

    {
        ImGui::Text("Rotation: ");
        ImGui::InputFloat("x1", &m_Rotation.x, 0.1f, 0.01f);
        ImGui::InputFloat("y1", &m_Rotation.y, 0.1f, 0.01f);
        ImGui::InputFloat("z1", &m_Rotation.z, 0.1f, 0.01f);
    }

    {
        ImGui::Text("Render Rotation: ");
        ImGui::InputFloat("lx", &m_RenderRotation.x, 0.1f, 0.01f);
        ImGui::InputFloat("ly", &m_RenderRotation.y, 0.1f, 0.01f);
        ImGui::InputFloat("lz", &m_RenderRotation.z, 0.1f, 0.01f);
    }

    {
        ImGui::Text("Scale: ");
        ImGui::InputFloat3("xyz", glm::value_ptr(m_Scale));
    }

    // ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::End();
}

} // namespace Engine
