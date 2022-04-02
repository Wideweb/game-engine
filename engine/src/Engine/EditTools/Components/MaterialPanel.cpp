#include "MaterialPanel.hpp"

#include "Configs.hpp"
#include "File.hpp"
#include "Texture.hpp"
#include "TextureLoader.hpp"

#include "imgui/imgui.h"
#include <filesystem>
#include <memory>

namespace Engine {

MaterialPanel::MaterialPanel(GameObjectModel &model) : m_Model(model) {}

void MaterialPanel::onDraw(int x, int y) {
    ImGui::Begin("Material");

    {
        float prevShininess = m_Model.material().shininess;
        float shininess = m_Model.material().shininess;
        ImGui::InputFloat("shininess", &shininess, 0.1f, 0.01f);
        if (prevShininess != shininess) {
            m_Model.materialShininess(shininess);
        }

        float prevSpecular = m_Model.material().specular;
        float specular = m_Model.material().specular;
        ImGui::InputFloat("specular", &specular, 0.1f, 0.01f);
        if (prevSpecular != specular) {
            m_Model.materialShininess(specular);
        }

        ImGui::Button("Diffuse map", ImVec2(100.0f, 0.0f));
        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
                const char *path = static_cast<const char *>(payload->Data);
                if (File::isExtension(path, Configs::c_TextureExtensions)) {
                    std::filesystem::path textureSrc = std::filesystem::path("assets") / path;
                    m_Model.setDiffuseMap(TextureLoader::loadTexture(textureSrc));
                }
            }
            ImGui::EndDragDropTarget();
        }

        ImGui::Button("Specular map", ImVec2(100.0f, 0.0f));
        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
                const char *path = static_cast<const char *>(payload->Data);
                if (File::isExtension(path, Configs::c_TextureExtensions)) {
                    std::filesystem::path textureSrc = std::filesystem::path("assets") / path;
                    m_Model.setSpecualMap(TextureLoader::loadTexture(textureSrc));
                }
            }
            ImGui::EndDragDropTarget();
        }

        ImGui::Button("Noraml map", ImVec2(100.0f, 0.0f));
        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
                const char *path = static_cast<const char *>(payload->Data);
                if (File::isExtension(path, Configs::c_TextureExtensions)) {
                    std::filesystem::path textureSrc = std::filesystem::path("assets") / path;
                    m_Model.setNormalMap(TextureLoader::loadTexture(textureSrc));
                }
            }
            ImGui::EndDragDropTarget();
        }
    }

    ImGui::End();
}

} // namespace Engine
