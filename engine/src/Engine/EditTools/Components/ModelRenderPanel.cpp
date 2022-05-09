#include "ModelRenderPanel.hpp"

#include "Configs.hpp"
#include "File.hpp"
#include "ImGuiWidgets.hpp"
#include "ModelLoader.hpp"
#include "Render3DComponent.hpp"
#include "SkinnedModel.hpp"
#include "Texture.hpp"
#include "TextureLoader.hpp"

#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>

#include <filesystem>
#include <memory>

namespace Engine {

ModelRenderPanel::ModelRenderPanel(GameObjectModel &model) : m_Model(model) {}

void ModelRenderPanel::onUpdate() {
    if (m_FreeDiffuseMap) {
        m_Model.setDiffuseMap(Texture::getEmpty());
        m_FreeDiffuseMap = false;
    }

    if (m_FreeSpecularMap) {
        m_Model.setSpecularMap(Texture::getEmpty());
        m_FreeSpecularMap = false;
    }

    if (m_FreeNormalMap) {
        m_Model.setNormalMap(Texture::getEmpty());
        m_FreeNormalMap = false;
    }
}

void ModelRenderPanel::onDraw() {
    auto &coordinator = gameLayer().getCoordinator();
    auto &render = m_Model.render();

    static bool expanded = false;
    if (!ImGuiWidgets::ComponentPanel<Render3DComponent>("3D Model", expanded, m_Model.entity(), coordinator, true)) {
        return;
    }

    float padding = 10.0f;

    ImGuiWidgets::PaddingLeft(padding);
    ImGui::Text("Model");

    ImGuiWidgets::PaddingLeft(padding);
    ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() - padding);
    if (ImGui::BeginCombo("##ModelRender_model", render.model.c_str())) {
        if (ImGui::Selectable("", render.model.empty())) {
            m_Model.set3DModel("");
        }
        const auto &models = Application::get().getModels().keys();
        for (const auto &model : models) {
            if (model.rfind(Configs::c_EditToolsModelPrefix, 0) == 0) {
                continue;
            }
            bool isSelected = model == render.model;
            if (ImGui::Selectable(model.c_str(), isSelected)) {
                m_Model.set3DModel(model);
            }
        }
        ImGui::EndCombo();
    }

    ImGui::PushItemWidth(120.0f);

    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
            const char *path = static_cast<const char *>(payload->Data);
            if (File::isExtension(path, Configs::c_3DModelExtensions)) {
                auto &models = Application::get().getModels();
                if (!models.HasModel(path)) {
                    std::filesystem::path modelSrc = std::filesystem::path("assets") / path;
                    models.RegisterModel(path, ModelLoader::load(modelSrc));
                }

                m_Model.set3DModel(path);
            }
        }
        ImGui::EndDragDropTarget();
    }

    if (!render.model.empty()) {

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::Text("Material");

        {
            float prevShininess = m_Model.material().shininess;
            float shininess = m_Model.material().shininess;
            ImGuiWidgets::PaddingLeft(padding);
            ImGui::InputFloat("Shininess", &shininess, 0.1f, 0.01f);
            if (prevShininess != shininess) {
                m_Model.materialShininess(shininess);
            }

            float prevSpecular = m_Model.material().specular;
            float specular = m_Model.material().specular;
            ImGuiWidgets::PaddingLeft(padding);
            ImGui::InputFloat("Specular", &specular, 0.1f, 0.01f);
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
            if (!material.diffuseMap.empty()) {
                ImGui::SameLine();
                ImGuiWidgets::PaddingLeft(-24.0f);
                ImGuiWidgets::PaddingTop(2.0f);
                ImGui::PushID("##ModelRender_diffuseMap_clear");
                if (ImGui::SmallButton("x")) {
                    m_FreeDiffuseMap = true;
                }
                ImGui::PopID();
                ImGuiWidgets::PaddingTop(-2.0f);
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
            if (!material.specularMap.empty()) {
                ImGui::SameLine();
                ImGuiWidgets::PaddingLeft(-24.0f);
                ImGuiWidgets::PaddingTop(2.0f);
                ImGui::PushID("##ModelRender_specularMap_clear");
                if (ImGui::SmallButton("x")) {
                    m_FreeSpecularMap = true;
                }
                ImGui::PopID();
                ImGuiWidgets::PaddingTop(-2.0f);
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
            if (!material.normalMap.empty()) {
                ImGui::SameLine();
                ImGuiWidgets::PaddingLeft(-24.0f);
                ImGuiWidgets::PaddingTop(2.0f);
                ImGui::PushID("##ModelRender_normalMap_clear");
                if (ImGui::SmallButton("x")) {
                    m_FreeNormalMap = true;
                }
                ImGui::PopID();
                ImGuiWidgets::PaddingTop(-2.0f);
            }
        }

        {
            glm::vec3 prevRotation = m_Model.renderRotation();
            glm::vec3 rotation = prevRotation;

            ImGuiWidgets::PaddingLeft(padding);
            ImGui::Text("Rotation: ");

            ImGuiWidgets::PaddingLeft(padding);
            ImGui::InputFloat("##ModelRender_rotationX", &rotation.x, 0.1f, 0.01f);
            ImGui::SameLine();
            ImGui::Text("X");

            ImGuiWidgets::PaddingLeft(padding);
            ImGui::InputFloat("##ModelRender_rotationY", &rotation.y, 0.1f, 0.01f);
            ImGui::SameLine();
            ImGui::Text("Y");

            ImGuiWidgets::PaddingLeft(padding);
            ImGui::InputFloat("##ModelRender_rotationZ", &rotation.z, 0.1f, 0.01f);
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
            ImGui::InputFloat("##ModelRender_scaleX", &scale.x, 0.1f, 0.01f);
            ImGui::SameLine();
            ImGui::Text("X");

            ImGuiWidgets::PaddingLeft(padding);
            ImGui::InputFloat("##ModelRender_scaleY", &scale.y, 0.1f, 0.01f);
            ImGui::SameLine();
            ImGui::Text("Y");

            ImGuiWidgets::PaddingLeft(padding);
            ImGui::InputFloat("##ModelRender_scaleZ", &scale.z, 0.1f, 0.01f);
            ImGui::SameLine();
            ImGui::Text("Z");

            if (scale != prevScale) {
                m_Model.renderScale(scale);
            }
        }
    }

    ImGui::PopItemWidth();
    ImGui::NewLine();

    // ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

} // namespace Engine
