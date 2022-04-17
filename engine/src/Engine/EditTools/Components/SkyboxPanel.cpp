#include "SkyboxPanel.hpp"

#include "Application.hpp"
#include "Configs.hpp"
#include "File.hpp"
#include "ImGuiWidgets.hpp"
#include "ModelLoader.hpp"
#include "Texture.hpp"
#include "TextureLoader.hpp"

#include "glad/glad.h"
#include "imgui/imgui.h"
#include <filesystem>
#include <memory>

namespace Engine {

void SkyboxPanel::onDraw() {
    static bool expanded = false;
    if (!ImGuiWidgets::CollapsePanel("Skybox", expanded)) {
        return;
    }

    ImGui::PushItemWidth(120.0f);
    float padding = 10.0f;

    auto &scene = gameLayer().getScene();

    bool useCubeMap = !!scene.getSkybox();
    ImGuiWidgets::PaddingLeft(padding);
    ImGui::Checkbox("CubeMap", &useCubeMap);
    if (useCubeMap && !scene.getSkybox()) {
        scene.setSkybox(ModelLoader::loadSkybox({}));
        auto &cubemap = scene.getSkybox()->cubemapTexture;
        for (size_t i = 0; i < 6; i++) {
            cubemap.faces[i] = TextureLoader::placeholder();
        }
    }
    if (!useCubeMap && !!scene.getSkybox()) {
        scene.getSkybox()->cubemapTexture.free();
        scene.setSkybox(nullptr);
    }

    if (gameLayer().getScene().getSkybox()) {
        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("Upper Limit", &gameLayer().getScene().getSkybox()->upperLimit, 0.01f);

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("Lower Limit", &gameLayer().getScene().getSkybox()->lowerLimit, 0.01f);

        static float rotation = glm::degrees(gameLayer().getScene().getSkybox()->rotation);
        // if (!Application::get().getTime().poused()) {
        //     rotation = glm::degrees(gameLayer().getScene().getSkybox()->rotation);
        // }
        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("Rotation", &rotation, 1.0);
        if (Application::get().getTime().poused()) {
            gameLayer().getScene().getSkybox()->rotation = glm::radians(rotation);
        }

        static float rotationSpeed = glm::degrees(gameLayer().getScene().getSkybox()->rotationSpeed);
        // if (!Application::get().getTime().poused()) {
        //     rotationSpeed = glm::degrees(gameLayer().getScene().getSkybox()->rotationSpeed);
        // }
        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("Rotation Speed", &rotationSpeed);
        if (Application::get().getTime().poused()) {
            gameLayer().getScene().getSkybox()->rotationSpeed = glm::radians(rotationSpeed);
        }

        ImGuiWidgets::PaddingTop(5.0f);
        ImGuiWidgets::PaddingLeft(89.0f);
        showFace(2);

        ImGuiWidgets::PaddingLeft(padding);
        showFace(1);
        ImGui::SameLine();
        ImGuiWidgets::PaddingLeft(-4.0f);
        showFace(4);
        ImGui::SameLine();
        ImGuiWidgets::PaddingLeft(-4.0f);
        showFace(0);
        ImGui::SameLine();
        ImGuiWidgets::PaddingLeft(-4.0f);
        showFace(5);

        ImGuiWidgets::PaddingLeft(89.0f);
        showFace(3);
    } else {
        glm::vec4 prevClearColor = Application::get().getRender().getClearColor();
        glm::vec4 clearColor = prevClearColor;
        ImGuiWidgets::PaddingLeft(padding);
        ImGui::ColorEdit3("Color", glm::value_ptr(clearColor));
        if (clearColor != prevClearColor) {
            Application::get().getRender().setClearColor(clearColor);
        }
    }

    ImGui::PopItemWidth();
    ImGui::NewLine();
}

void SkyboxPanel::showFace(size_t index) {
    auto &cubemap = gameLayer().getScene().getSkybox()->cubemapTexture;
    GfxObjectId id = !cubemap.faces[index].empty() ? cubemap.faces[index].id : TextureLoader::placeholder().id;

    ImGui::Image(reinterpret_cast<void *>(cubemap.faces[index].id), ImVec2(75, 75), ImVec2(0, 0), ImVec2(1, 1));
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
            const char *path = static_cast<const char *>(payload->Data);
            if (File::isExtension(path, Configs::c_TextureExtensions)) {
                std::filesystem::path textureSrc = std::filesystem::path("assets") / path;

                if (!cubemap.faces[index].empty() && cubemap.faces[index].id != TextureLoader::placeholder().id) {
                    cubemap.faces[index].free();
                }
                TextureLoader::loadCubemap(cubemap, textureSrc, index);
            }
        }
        ImGui::EndDragDropTarget();
    }
}

} // namespace Engine
