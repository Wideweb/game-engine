#include "GamePanel.hpp"

#include "Application.hpp"
#include "CameraComponent.hpp"
#include "LocationComponent.hpp"

#include "glad/glad.h"
#include "imgui/imgui.h"
#include <glm/gtx/quaternion.hpp>

namespace Engine {

GamePanel::GamePanel() {}

void GamePanel::onAttach() {
    auto &window = Application::get().getWindow();
    auto &render = Application::get().getRender();

    m_ViewportSize = {window.getWidth(), window.getHeight()};
    m_PrevViewportSize = m_ViewportSize;
    m_Ratio = m_ViewportSize.x / m_ViewportSize.y;

    m_GColor = Texture::createRGBA32FBuffer(m_ViewportSize.x, m_ViewportSize.y);
    m_GEntity = Texture::createR32IBuffer(m_ViewportSize.x, m_ViewportSize.y);
    m_GDepth = Texture::createDepthBuffer(m_ViewportSize.x, m_ViewportSize.y);

    m_Framebuffer = Framebuffer::create();
    m_Framebuffer.bind();
    m_Framebuffer.addAttachment(m_GColor);
    m_Framebuffer.addAttachment(m_GEntity);
    m_Framebuffer.addAttachment(m_GDepth);
    m_Framebuffer.check();
    m_Framebuffer.unbind();
}

void GamePanel::onUpdate() {
    if (m_PrevViewportSize != m_ViewportSize) {
        m_GColor.bind();
        m_GColor.resize(m_ViewportSize.x, m_ViewportSize.y);

        m_GEntity.bind();
        m_GEntity.resize(m_ViewportSize.x, m_ViewportSize.y);

        m_GDepth.bind();
        m_GDepth.resize(m_ViewportSize.x, m_ViewportSize.y);
        m_GDepth.unbind();

        m_PrevViewportSize = m_ViewportSize;
    }
}

void GamePanel::onDraw() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
    ImGui::Begin("Camera");

    auto &render = Application::get().getRender();
    auto &camera = Application::get().getCamera();
    auto &models = Application::get().getModels();
    auto &scene = gameLayer().getScene();
    auto &renderSettings = gameLayer().renderSettings;
    auto &coordinator = gameLayer().getCoordinator();

    glm::vec3 lastCameraPos = camera.positionVec();
    glm::quat lastCameraRotation = camera.rotationQuat();
    glm::vec2 lastCameraSize = camera.size();
    Framebuffer lastFBO = render.getFramebuffer();
    auto lastViewport = render.getViewport();

    auto cameraArray = coordinator.GetComponentArray<CameraComponent>();
    if (cameraArray->size() > 0) {
        Entity entity = cameraArray->entities()[0];
        auto &entityLocation = coordinator.GetComponent<LocationComponent>(entity);
        auto &entityCamera = coordinator.GetComponent<CameraComponent>(entity);

        auto model = LocationComponent::getFullTransform(entity, coordinator.GetComponentManager());

        glm::vec3 scale;
        glm::quat rotation;
        glm::vec3 position;
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(model, scale, rotation, position, skew, perspective);

        camera.setPosition(position);
        camera.setRotation(rotation);
        camera.setSize(m_ViewportSize.x, m_ViewportSize.y);

        render.setFramebuffer(m_Framebuffer);
        render.setViewport(m_ViewportSize.x, m_ViewportSize.y);
        render.clear();
        render.draw(camera, scene, models, renderSettings);
        render.setFramebuffer(lastFBO);
        render.setViewport(lastViewport.width, lastViewport.height);

        camera.setPosition(lastCameraPos);
        camera.setRotation(lastCameraRotation);
        camera.setSize(lastCameraSize.x, lastCameraSize.y);
    } else {
        render.setFramebuffer(m_Framebuffer);
        render.setViewport(m_ViewportSize.x, m_ViewportSize.y);
        render.clear();
        render.setFramebuffer(lastFBO);
        render.setViewport(lastViewport.width, lastViewport.height);
    }

    // auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
    // auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
    // auto viewportOffset = ImGui::GetWindowPos();
    // glm::vec2 min = {viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y};
    // glm::vec2 max = {viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y};

    // m_BlockEvents = !ImGui::IsWindowHovered();

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    m_ViewportSize = {viewportPanelSize.x, viewportPanelSize.y};
    // Application::get().getMousePicker().setRect(min, min + m_ViewportSize);

    ImGui::Image(reinterpret_cast<void *>(m_GColor.id), ImVec2{m_ViewportSize.x, m_ViewportSize.y}, ImVec2{0, 1},
                 ImVec2{1, 0});

    ImGui::End();
    ImGui::PopStyleVar();
}

} // namespace Engine
