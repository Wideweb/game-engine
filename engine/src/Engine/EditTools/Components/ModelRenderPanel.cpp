#include "ModelRenderPanel.hpp"

#include "Configs.hpp"
#include "File.hpp"
#include "ImGuiWidgets.hpp"
#include "Render3DComponent.hpp"
#include "Texture.hpp"
#include "TextureLoader.hpp"

#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>

#include <filesystem>
#include <memory>

namespace Engine {

ModelRenderPanel::ModelRenderPanel(GameObjectModel &model) : m_Model(model) {}

void ModelRenderPanel::onDraw(int x, int y) {
    static bool expanded = false;
    ImGuiWidgets::ComponentPanel<Render3DComponent>("Model Render", expanded, m_Model.entity(),
                                                    gameLayer().getCoordinator(), true);
    if (!expanded) {
        return;
    }

    ImGui::PushItemWidth(120.0f);
    float padding = 10.0f;

    ImGuiWidgets::PaddingLeft(padding);
    ImGui::Text("Material");

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
