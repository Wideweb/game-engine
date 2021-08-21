#include "MaterialPanel.hpp"

#include "Texture.hpp"
#include "TextureLoader.hpp"

#include "imgui/imgui.h"
#include <filesystem>
#include <memory>

namespace Engine {

MaterialPanel::MaterialPanel(GameObjectModel &model) : m_Model(model) {}

void MaterialPanel::onAttach() {
    m_Model.materialShininessChange$.addEventCallback([this](float scale) { m_Shininess = scale; });
    m_Model.materialSpecularChange$.addEventCallback([this](float scale) { m_Specular = scale; });
}

void MaterialPanel::onUpdate() {
    if (m_Shininess != m_Model.material().shininess) {
        m_Model.materialShininess(m_Shininess);
    }

    if (m_Specular != m_Model.material().specular) {
        m_Model.materialSpecular(m_Specular);
    }
}

void MaterialPanel::onDraw(int x, int y) {
    ImGui::Begin("Material");

    {
        ImGui::InputFloat("shininess", &m_Shininess, 0.1f, 0.01f);
        ImGui::InputFloat("specular", &m_Specular, 0.1f, 0.01f);

        ImGui::Button("Diffuse map", ImVec2(100.0f, 0.0f));
        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
                const char *path = static_cast<const char *>(payload->Data);
                std::filesystem::path textureSrc = std::filesystem::path("assets") / path;
                m_Model.setDiffuseMap(std::shared_ptr<Texture>(TextureLoader::loadTexture(textureSrc)));
            }
            ImGui::EndDragDropTarget();
        }

        ImGui::Button("Specular map", ImVec2(100.0f, 0.0f));
        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
                const char *path = static_cast<const char *>(payload->Data);
                std::filesystem::path textureSrc = std::filesystem::path("assets") / path;
                m_Model.setSpecualMap(std::shared_ptr<Texture>(TextureLoader::loadTexture(textureSrc)));
            }
            ImGui::EndDragDropTarget();
        }

        ImGui::Button("Noraml map", ImVec2(100.0f, 0.0f));
        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
                const char *path = static_cast<const char *>(payload->Data);
                std::filesystem::path textureSrc = std::filesystem::path("assets") / path;
                m_Model.setNormalMap(std::shared_ptr<Texture>(TextureLoader::loadTexture(textureSrc)));
            }
            ImGui::EndDragDropTarget();
        }
    }

    ImGui::End();
}

} // namespace Engine
