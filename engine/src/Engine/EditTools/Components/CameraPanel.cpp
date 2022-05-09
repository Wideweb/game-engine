#include "CameraPanel.hpp"

#include "CameraComponent.hpp"
#include "ImGuiWidgets.hpp"

#include "Application.hpp"
#include "SkinnedModel.hpp"

#include "imgui/imgui.h"

namespace Engine {

CameraPanel::CameraPanel(GameObjectModel &model) : m_Model(model) {}

void CameraPanel::onDraw() {
    static bool expanded = false;
    if (!ImGuiWidgets::ComponentPanel<CameraComponent>("Camera", expanded, m_Model.entity(),
                                                       gameLayer().getCoordinator(), true)) {
        return;
    }

    auto &camera = Application::get().getCamera();
    auto &cameraCmp = gameLayer().getCoordinator().GetComponent<CameraComponent>(m_Model.entity());

    float padding = 10.0f;

    auto currentProjection = camera.getProjection() == Camera::Projection::PERSPECTIVE ? "Perspective" : "Orthogonal";

    ImGuiWidgets::PaddingLeft(padding);
    ImGui::Text("Projection");
    ImGui::PushItemWidth(120.0f);
    ImGuiWidgets::PaddingLeft(padding);
    if (ImGui::BeginCombo("##Camera_Projection", currentProjection)) {
        if (ImGui::Selectable("Perspective", camera.getProjection() == Camera::Projection::PERSPECTIVE)) {
            camera.setProjection(Camera::Projection::PERSPECTIVE);
            camera.setPerspective(camera.getFieldOfView(), camera.getZNear(), camera.getZFar());
        }

        if (ImGui::Selectable("Orthogonal", camera.getProjection() == Camera::Projection::ORTHOGONAL)) {
            camera.setProjection(Camera::Projection::ORTHOGONAL);
            camera.setOrthogonal(camera.getZNear(), camera.getZFar(), camera.getZoom());
        }

        ImGui::EndCombo();
    }

    {
        float prev = camera.getZNear();
        float current = prev;
        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("Near Plane", &current, 0.1f);
        if (prev != current) {
            camera.setPerspective(camera.getFieldOfView(), current, camera.getZFar());
        }
    }

    {
        float prev = camera.getZFar();
        float current = prev;
        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("Far Plane", &current, 0.1f);
        if (prev != current) {
            camera.setPerspective(camera.getFieldOfView(), camera.getZNear(), current);
        }
    }

    if (camera.getProjection() == Camera::Projection::PERSPECTIVE) {

        {
            float prev = glm::degrees(camera.getFieldOfView());
            float current = prev;
            ImGuiWidgets::PaddingLeft(padding);
            ImGui::SliderFloat("##Camera_FieldOfView", &current, 1.0f, 179.0f);
            ImGui::SameLine();
            ImGui::Text("Field of View");
            if (prev != current) {
                camera.setPerspective(glm::radians(current), camera.getZNear(), camera.getZFar());
            }
        }
    }

    if (camera.getProjection() == Camera::Projection::ORTHOGONAL) {
        {
            float prev = camera.getZoom();
            float current = prev;

            ImGuiWidgets::PaddingLeft(padding);
            ImGui::SliderFloat("##Camera_Zoom", &current, 0.01f, 100.0f);
            ImGui::SameLine();
            ImGui::Text("Zoom");

            if (prev != current) {
                camera.setZoom(current);
            }
        }
    }

    ImGui::PopItemWidth();
    ImGui::NewLine();
}

} // namespace Engine
