#include "ScenePanel.hpp"

#include "Application.hpp"
#include "Configs.hpp"
#include "File.hpp"
#include "Location2DComponent.hpp"
#include "LocationComponent.hpp"
#include "ModelFactory.hpp"
#include "ModelLoader.hpp"
#include "Render3DComponent.hpp"
#include "SkeletComponent.hpp"
#include "SkinnedModel.hpp"
#include "TagComponent.hpp"

#include "glad/glad.h"
#include "imgui/imgui.h"
#include <filesystem>
#include <glm/gtx/quaternion.hpp>

namespace Engine {

ScenePanel::ScenePanel() {}

void ScenePanel::onAttach() {
    auto &window = Application::get().getWindow();
    auto &render = Application::get().getRender();

    m_ViewportSize = {window.getWidth(), window.getHeight()};
    m_PrevViewportSize = m_ViewportSize;
    m_Ratio = m_ViewportSize.x / m_ViewportSize.y;

    m_GColor = Texture::createRGBA32FBuffer(m_ViewportSize.x, m_ViewportSize.y);
    m_GEntity = Texture::createR32IBuffer(m_ViewportSize.x, m_ViewportSize.y);
    m_GDepth = Renderbuffer::create(m_ViewportSize.x, m_ViewportSize.y, Renderbuffer::InternalFormat::DEPTH_COMPONENT);

    m_Framebuffer = Framebuffer::create();
    m_Framebuffer.bind();
    m_Framebuffer.addAttachment(m_GColor);
    m_Framebuffer.addAttachment(m_GEntity);
    m_Framebuffer.setDepthAttachment(m_GDepth);
    m_Framebuffer.check();
    m_Framebuffer.unbind();

    render.setFramebuffer(m_Framebuffer);
    render.clear();
}

void ScenePanel::onUpdate() {
    if (m_PrevViewportSize != m_ViewportSize) {

        m_GColor.bind();
        m_GColor.resize(m_ViewportSize.x, m_ViewportSize.y);

        m_GEntity.bind();
        m_GEntity.resize(m_ViewportSize.x, m_ViewportSize.y);

        m_GDepth.bind();
        m_GDepth.resize(m_ViewportSize.x, m_ViewportSize.y);
        m_GDepth.unbind();

        auto &render = Application::get().getRender();
        auto &camera = Application::get().getCamera();

        render.setViewport(m_ViewportSize.x, m_ViewportSize.y);
        render.clear();
        camera.setSize(m_ViewportSize.x, m_ViewportSize.y);

        m_PrevViewportSize = m_ViewportSize;
    }
}

void ScenePanel::onMouseEvent(MouseEvent &event) {
    auto pos = Application::get().getMousePicker().getPos();

    m_Framebuffer.bind();
    m_Framebuffer[1].read(pos.x, m_ViewportSize.y - pos.y, 1, 1, &m_Entity);
    m_Framebuffer.unbind();

    event.data = &m_Entity;
}

void ScenePanel::onDraw() {
    auto &coordinator = gameLayer().getCoordinator();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
    ImGui::Begin("Scene");

    auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
    auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
    auto viewportOffset = ImGui::GetWindowPos();
    glm::vec2 min = {viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y};
    glm::vec2 max = {viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y};

    m_BlockEvents = !ImGui::IsWindowHovered();

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    m_ViewportSize = {viewportPanelSize.x, viewportPanelSize.y};

    Application::get().getMousePicker().setRect(min, min + m_ViewportSize);
    Materials& materials = Application::get().getMaterials();

    ImGui::Image(reinterpret_cast<void *>(m_GColor.id), ImVec2{m_ViewportSize.x, m_ViewportSize.y}, ImVec2{0, 1},
                 ImVec2{1, 0});
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
            auto path = std::string(static_cast<const char *>(payload->Data));

            if (File::isExtension(path, Configs::c_SceneObjectExtensions)) {
                auto &models = Application::get().getModels();
                if (!models.HasModel(path)) {
                    std::filesystem::path modelSrc = std::filesystem::path("assets") / path;
                    models.RegisterModel(path, ModelLoader::load(modelSrc));
                }

                std::string name;
                size_t index = coordinator.GetEntities().size();
                do {
                    name = std::string("entity") + std::to_string(index);
                    index++;
                } while (coordinator.HasEntity(name));
                auto entity = coordinator.CreateEntity(name);

                coordinator.AddComponent<LocationComponent>(entity, LocationComponent(glm::vec3(0.0f)));
                coordinator.AddComponent<Render3DComponent>(entity, Render3DComponent(path, materials.defaultMaterial.get()));
                coordinator.AddComponent<TagComponent>(entity, TagComponent(name));

                if (models.Is<SkinnedModel>(path)) {
                    coordinator.AddComponent<SkeletComponent>(entity, SkeletComponent());
                }
            }
        }
        ImGui::EndDragDropTarget();
    }

    if (ImGui::BeginPopupContextWindow()) {
        if (ImGui::MenuItem("Create Empty")) {
            std::string name;
            size_t index = coordinator.GetEntities().size();
            do {
                name = std::string("entity") + std::to_string(index);
                index++;
            } while (coordinator.HasEntity(name));
            auto entity = coordinator.CreateEntity(name);

            coordinator.AddComponent<LocationComponent>(entity, LocationComponent(glm::vec3(0.0f)));
            coordinator.AddComponent<TagComponent>(entity, TagComponent(name));
        }

        if (ImGui::MenuItem("Create Empty 2D")) {
            std::string name;
            size_t index = coordinator.GetEntities().size();
            do {
                name = std::string("entity") + std::to_string(index);
                index++;
            } while (coordinator.HasEntity(name));
            auto entity = coordinator.CreateEntity(name);

            coordinator.AddComponent<Location2DComponent>(entity, Location2DComponent());
            coordinator.AddComponent<TagComponent>(entity, TagComponent(name));
        }

        if (ImGui::MenuItem("Add Box")) {
            std::string model = "primitive-box";
            auto &models = Application::get().getModels();
            if (!models.HasModel(model)) {
                models.RegisterModel(model, ModelFactory::createCube());
            }

            std::string name;
            size_t index = coordinator.GetEntities().size();
            do {
                name = std::string("entity") + std::to_string(index);
                index++;
            } while (coordinator.HasEntity(name));
            auto entity = coordinator.CreateEntity(name);

            coordinator.AddComponent<LocationComponent>(entity, LocationComponent(glm::vec3(0.0f)));
            coordinator.AddComponent<Render3DComponent>(entity, Render3DComponent(model, materials.defaultMaterial.get()));
            coordinator.AddComponent<TagComponent>(entity, TagComponent(name));
        }

        if (ImGui::MenuItem("Add Plane")) {
            std::string model = "primitive-plane";
            auto &models = Application::get().getModels();
            if (!models.HasModel(model)) {
                models.RegisterModel(model, ModelFactory::createPlane());
            }

            std::string name;
            size_t index = coordinator.GetEntities().size();
            do {
                name = std::string("entity") + std::to_string(index);
                index++;
            } while (coordinator.HasEntity(name));
            auto entity = coordinator.CreateEntity(name);
            coordinator.AddComponent<LocationComponent>(entity, LocationComponent(glm::vec3(0.0f)));
            coordinator.AddComponent<Render3DComponent>(entity, Render3DComponent(model, materials.defaultMaterial.get()));
            coordinator.AddComponent<TagComponent>(entity, TagComponent(name));
        }
        ImGui::EndPopup();
    }

    ImGui::End();
    ImGui::PopStyleVar();
}

} // namespace Engine
