#include "SkyboxPanel.hpp"

#include "Configs.hpp"
#include "File.hpp"
#include "ModelLoader.hpp"
#include "Texture.hpp"
#include "TextureLoader.hpp"

#include "Application.hpp"

#include "glad/glad.h"
#include "imgui/imgui.h"
#include <filesystem>
#include <memory>

namespace Engine {

void SkyboxPanel::onDraw(int x, int y) {
    auto &scene = gameLayer().getScene();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{10, 10});
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{5, 5});

    ImGui::Begin("Skybox");

    bool useCubeMap = !!scene.getSkybox();
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
        static float rotation = 0;
        if (!Application::get().getTime().poused()) {
            rotation = gameLayer().getScene().getSkybox()->rotation;
        }
        ImGui::InputFloat("rotation", &rotation, 1.0);
        if (Application::get().getTime().poused()) {
            gameLayer().getScene().getSkybox()->rotation = glm::radians(rotation);
        }

        static float rotationSpeed = 0;
        if (!Application::get().getTime().poused()) {
            rotationSpeed = gameLayer().getScene().getSkybox()->rotationSpeed;
        }
        ImGui::InputFloat("rotation speed", &rotationSpeed);
        if (Application::get().getTime().poused()) {
            gameLayer().getScene().getSkybox()->rotationSpeed = rotationSpeed;
        }

        ImGui::SetCursorPosX(90.0f);
        showFace(2);

        showFace(1);
        ImGui::SameLine();
        showFace(4);
        ImGui::SameLine();
        showFace(0);
        ImGui::SameLine();
        showFace(5);

        ImGui::SetCursorPosX(90.0f);
        showFace(3);
    } else {
        glm::vec4 prevClearColor = Application::get().getRender().getClearColor();
        glm::vec4 clearColor = prevClearColor;
        ImGui::ColorEdit3("Color", glm::value_ptr(clearColor));
        if (clearColor != prevClearColor) {
            Application::get().getRender().setClearColor(clearColor);
        }
    }

    ImGui::End();

    ImGui::PopStyleVar(2);
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
