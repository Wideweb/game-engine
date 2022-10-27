#include "ModelRenderPanel.hpp"

#include "Configs.hpp"
#include "File.hpp"
#include "ImGuiWidgets.hpp"
#include "ModelLoader.hpp"
#include "Render3DComponent.hpp"
#include "SkinnedModel.hpp"
#include "Texture.hpp"
#include "TextureLoader.hpp"
#include "Material.hpp"

#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>

#include <filesystem>
#include <memory>
#include <string>

namespace Engine {

ModelRenderPanel::ModelRenderPanel(GameObjectModel &model) : m_Model(model) {}

void ModelRenderPanel::onDraw() {
    auto &coordinator = gameLayer().getCoordinator();
    auto &render = m_Model.render();

    static bool expanded = false;
    if (!ImGuiWidgets::ComponentPanel<Render3DComponent>("3D Model", expanded, m_Model.entity(), coordinator, true)) {
        return;
    }

    ImGuiWidgets::PaddingLeft(m_Padding);
    ImGui::Text("Model");

    ImGuiWidgets::PaddingLeft(m_Padding);
    ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() - m_Padding);
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

    showMaterialProperties(render.material);

    if (!render.model.empty()) {
        {
            glm::vec3 prevRotation = m_Model.renderRotation();
            glm::vec3 rotation = prevRotation;

            ImGuiWidgets::PaddingLeft(m_Padding);
            ImGui::Text("Rotation: ");

            ImGuiWidgets::PaddingLeft(m_Padding);
            ImGui::InputFloat("##ModelRender_rotationX", &rotation.x, 0.1f, 0.01f);
            ImGui::SameLine();
            ImGui::Text("X");

            ImGuiWidgets::PaddingLeft(m_Padding);
            ImGui::InputFloat("##ModelRender_rotationY", &rotation.y, 0.1f, 0.01f);
            ImGui::SameLine();
            ImGui::Text("Y");

            ImGuiWidgets::PaddingLeft(m_Padding);
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

            ImGuiWidgets::PaddingLeft(m_Padding);
            ImGui::Text("Scale: ");

            ImGuiWidgets::PaddingLeft(m_Padding);
            ImGui::InputFloat("##ModelRender_scaleX", &scale.x, 0.1f, 0.01f);
            ImGui::SameLine();
            ImGui::Text("X");

            ImGuiWidgets::PaddingLeft(m_Padding);
            ImGui::InputFloat("##ModelRender_scaleY", &scale.y, 0.1f, 0.01f);
            ImGui::SameLine();
            ImGui::Text("Y");

            ImGuiWidgets::PaddingLeft(m_Padding);
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

void ModelRenderPanel::showMaterialProperties(Material* material) {
    ImGuiWidgets::PaddingLeft(m_Padding);
    ImGui::Text("Material");

    size_t propertiesNumber = material->keys().size();

    for (size_t i = 0; i < propertiesNumber; i++) {
        auto& propertyName = material->keys()[i];
        auto& property = material->properties()[i];

        switch (property.type)
        {
        case Shader::Property::Type::TEXTURE:
            showTextureProperty(material, propertyName);
            break;

        case Shader::Property::Type::FLOAT1:
            showFloatProperty(material, propertyName);
            break;
        
        default:
            break;
        }
    }
}

void ModelRenderPanel::showTextureProperty(Material* material, const std::string& propertyName) {
    uint32_t textureId = TextureLoader::placeholder().id;
    bool empty = material->getProperty(propertyName).value.texture == nullptr;

    if (!empty) {
        textureId = material->getProperty(propertyName).value.texture->id;
    }

    ImGuiWidgets::PaddingLeft(m_Padding);
    ImGui::Text(propertyName.c_str());
    ImGuiWidgets::PaddingLeft(m_Padding);
    ImGui::Image(reinterpret_cast<void *>(textureId), ImVec2(75, 75), ImVec2(0, 0), ImVec2(1, 1));
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
            const char *path = static_cast<const char *>(payload->Data);
            if (File::isExtension(path, Configs::c_TextureExtensions)) {
                std::filesystem::path textureSrc = std::filesystem::path("assets") / path;
                material->setTexture(propertyName, Application::get().getTextures().loadIfNotExist(textureSrc));
            }
        }
        ImGui::EndDragDropTarget();
    }
    if (!empty) {
        ImGui::SameLine();
        ImGuiWidgets::PaddingLeft(-24.0f);
        ImGuiWidgets::PaddingTop(2.0f);
        std::string clearId = "##ModelRender_" + propertyName + "_clear";
        ImGui::PushID(clearId.c_str());
        if (ImGui::SmallButton("x")) {
            material->setTexture(propertyName, nullptr);
        }
        ImGui::PopID();
        ImGuiWidgets::PaddingTop(-2.0f);
    }
}

void ModelRenderPanel::showFloatProperty(Material* material, const std::string& propertyName) {
    float valueOld = material->getProperty(propertyName).value.float1;
    float valueNew = valueOld;

    ImGuiWidgets::PaddingLeft(m_Padding);
    ImGui::InputFloat(propertyName.c_str(), &valueNew, 0.1f, 0.01f);

    if (valueNew != valueOld) {
        material->setFloat(propertyName, valueNew);
    }
}

} // namespace Engine
