#include "MaterialPanel.hpp"

#include "Configs.hpp"
#include "File.hpp"
#include "ImGuiWidgets.hpp"
#include "Texture.hpp"
#include "TextureLoader.hpp"

#include "imgui/imgui.h"
#include <filesystem>
#include <memory>

namespace Engine {

MaterialPanel::MaterialPanel(GameObjectModel &model) : m_Model(model) {}

void MaterialPanel::onDraw(int x, int y) {
    static bool expanded = false;
    ImGuiWidgets::Collapse("Material", expanded);
    if (!expanded) {
        return;
    }

    ImGui::PushItemWidth(120.0f);

    float padding = 10.0f;

    {
        float prevShininess = m_Model.material().shininess;
        float shininess = m_Model.material().shininess;
        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("shininess", &shininess, 0.1f, 0.01f);
        if (prevShininess != shininess) {
            m_Model.materialShininess(shininess);
        }

        float prevSpecular = m_Model.material().specular;
        float specular = m_Model.material().specular;
        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("specular", &specular, 0.1f, 0.01f);
        if (prevSpecular != specular) {
            m_Model.materialShininess(specular);
        }

        const auto &material = m_Model.material();
        const GfxObjectId diffuseMapId =
            !material.diffuseMap.empty() ? material.diffuseMap.id : TextureLoader::placeholder().id;
        const GfxObjectId specularMapId =
            !material.specularMap.empty() ? material.specularMap.id : TextureLoader::placeholder().id;
        const GfxObjectId normalMapId =
            !material.normalMap.empty() ? material.normalMap.id : TextureLoader::placeholder().id;

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::Text("Diffuse map");
        ImGuiWidgets::PaddingLeft(padding);
        ImGui::Image(reinterpret_cast<void *>(diffuseMapId), ImVec2(75, 75), ImVec2(0, 0), ImVec2(1, 1));
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

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::Text("Specular map");
        ImGuiWidgets::PaddingLeft(padding);
        ImGui::Image(reinterpret_cast<void *>(specularMapId), ImVec2(75, 75), ImVec2(0, 0), ImVec2(1, 1));
        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
                const char *path = static_cast<const char *>(payload->Data);
                if (File::isExtension(path, Configs::c_TextureExtensions)) {
                    std::filesystem::path textureSrc = std::filesystem::path("assets") / path;
                    m_Model.setSpecularMap(TextureLoader::loadTexture(textureSrc));
                }
            }
            ImGui::EndDragDropTarget();
        }

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::Text("Noraml map");
        ImGuiWidgets::PaddingLeft(padding);
        ImGui::Image(reinterpret_cast<void *>(normalMapId), ImVec2(75, 75), ImVec2(0, 0), ImVec2(1, 1));
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

    ImGui::PopItemWidth();
    ImGui::NewLine();
}

} // namespace Engine
