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

    glGenFramebuffers(1, &m_FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

    glGenTextures(1, &m_GColor);
    glBindTexture(GL_TEXTURE_2D, m_GColor);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_ViewportSize.x, m_ViewportSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_GColor, 0);

    glGenTextures(1, &m_GEntity);
    glBindTexture(GL_TEXTURE_2D, m_GEntity);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, m_ViewportSize.x, m_ViewportSize.y, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE,
                 NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_GEntity, 0);

    glGenRenderbuffers(1, &m_GDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, m_GDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_ViewportSize.x, m_ViewportSize.y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_GDepth);

    unsigned int attachments[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2, attachments);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GamePanel::onUpdate() {
    if (m_PrevViewportSize != m_ViewportSize) {
        glBindTexture(GL_TEXTURE_2D, m_GColor);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_ViewportSize.x, m_ViewportSize.y, 0, GL_RGBA, GL_FLOAT, NULL);
        glBindTexture(GL_TEXTURE_2D, 0);

        glBindTexture(GL_TEXTURE_2D, m_GEntity);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, m_ViewportSize.x, m_ViewportSize.y, 0, GL_RED_INTEGER,
                     GL_UNSIGNED_BYTE, NULL);
        glBindTexture(GL_TEXTURE_2D, 0);

        glBindRenderbuffer(GL_RENDERBUFFER, m_GDepth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_ViewportSize.x, m_ViewportSize.y);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        m_PrevViewportSize = m_ViewportSize;
    }
}

void GamePanel::onDraw(int x, int y) {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
    ImGui::Begin("Game");

    auto &render = Application::get().getRender();
    auto &camera = Application::get().getCamera();
    auto &models = Application::get().getModels();
    auto &scene = gameLayer().getScene();
    auto &renderSettings = gameLayer().renderSettings;
    auto &coordinator = gameLayer().getCoordinator();

    glm::vec3 lastCameraPos = camera.positionVec();
    glm::quat lastCameraRotation = camera.rotationQuat();
    glm::vec2 lastCameraSize = camera.size();
    unsigned int lastFBO = render.getFBO();
    auto lastViewport = render.getViewport();

    auto cameraArray = coordinator.GetComponentArray<CameraComponent>();
    Entity entity = cameraArray->entities()[0];
    auto &entityLocation = coordinator.GetComponent<LocationComponent>(entity);
    auto &entityCamera = coordinator.GetComponent<CameraComponent>(entity);

    camera.setPosition(entityLocation.position + glm::quat(entityLocation.rotation) * entityCamera.offset);
    camera.setRotation(glm::quat(entityLocation.rotation) * glm::quat(entityCamera.rotation));

    render.setFBO(m_FBO);
    render.setViewport(m_ViewportSize.x, m_ViewportSize.y);
    camera.setSize(m_ViewportSize.x, m_ViewportSize.y);
    render.clear();
    render.draw(camera, scene, models, renderSettings);
    render.setFBO(lastFBO);
    render.setViewport(lastViewport.width, lastViewport.height);

    camera.setPosition(lastCameraPos);
    camera.setRotation(lastCameraRotation);
    camera.setSize(lastCameraSize.x, lastCameraSize.y);

    // auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
    // auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
    // auto viewportOffset = ImGui::GetWindowPos();
    // glm::vec2 min = {viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y};
    // glm::vec2 max = {viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y};

    // m_BlockEvents = !ImGui::IsWindowHovered();

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    m_ViewportSize = {viewportPanelSize.x, viewportPanelSize.y};
    // Application::get().getMousePicker().setRect(min, min + m_ViewportSize);

    ImGui::Image(reinterpret_cast<void *>(m_GColor), ImVec2{m_ViewportSize.x, m_ViewportSize.y}, ImVec2{0, 1},
                 ImVec2{1, 0});

    ImGui::End();
    ImGui::PopStyleVar();
}

} // namespace Engine
