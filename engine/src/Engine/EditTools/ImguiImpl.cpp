#include "ImguiImpl.hpp"

#include "glad/glad.h"

#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"

#include <filesystem>
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp>

#include "Input.hpp"
#include "LocationComponent.hpp"
#include "Mesh2D.hpp"
#include "ModelFactory.hpp"
#include "ModelLoader.hpp"
#include "Render3DComponent.hpp"
#include "SkeletComponent.hpp"
#include "SkinnedModel.hpp"
#include "TagComponent.hpp"
#include "TextureLoader.hpp"

namespace Engine {

void ImguiImpl::OnAttach() {
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

    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    float fontSize = 14.0f;
    io.Fonts->AddFontFromFileTTF("assets/fonts/OpenSans-Bold.ttf", fontSize);
    io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/OpenSans-Regular.ttf", fontSize);

    ImGui::StyleColorsDark();

    ImGuiStyle &style = ImGui::GetStyle();
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;

    auto &colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_WindowBg] = ImVec4{0.1f, 0.105f, 0.11f, 1.0f};

    // Headers
    colors[ImGuiCol_Header] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
    colors[ImGuiCol_HeaderHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
    colors[ImGuiCol_HeaderActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

    // Buttons
    colors[ImGuiCol_Button] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
    colors[ImGuiCol_ButtonHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
    colors[ImGuiCol_ButtonActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

    // Frame BG
    colors[ImGuiCol_FrameBg] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
    colors[ImGuiCol_FrameBgHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
    colors[ImGuiCol_FrameBgActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

    // Tabs
    colors[ImGuiCol_Tab] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    colors[ImGuiCol_TabHovered] = ImVec4{0.38f, 0.3805f, 0.381f, 1.0f};
    colors[ImGuiCol_TabActive] = ImVec4{0.28f, 0.2805f, 0.281f, 1.0f};
    colors[ImGuiCol_TabUnfocused] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};

    // Title
    colors[ImGuiCol_TitleBg] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    colors[ImGuiCol_TitleBgActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

    ImGui_ImplSDL2_InitForOpenGL(static_cast<SDL_Window *>(window.getNaviteWindow()), window.getContext());
    ImGui_ImplOpenGL3_Init("#version 330 core");

    window.setNativeEventCallback(
        [this](const void *e) { ImGui_ImplSDL2_ProcessEvent(static_cast<const SDL_Event *>(e)); });

    m_IconPlay = TextureLoader::loadTexture("assets/icons/play.png");
    m_IconStop = TextureLoader::loadTexture("assets/icons/stop.png");
}

void ImguiImpl::onUpdate() {
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

void ImguiImpl::OnDetach() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void ImguiImpl::onMouseEvent(MouseEvent &event) {
    auto pos = Application::get().getMousePicker().getPos();

    m_Framebuffer.bind();
    m_Framebuffer[1].read(pos.x, m_ViewportSize.y - pos.y, 1, 1, &m_Entity);
    m_Framebuffer.unbind();
}

void ImguiImpl::Begin() {
    auto &window = Application::get().getWindow();
    auto &render = Application::get().getRender();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    static bool dockspaceOpen = true;
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen) {
        ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                        ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the
    // pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
    ImGui::PopStyleVar();

    if (opt_fullscreen) {
        ImGui::PopStyleVar(2);
    }

    ImGuiIO &io = ImGui::GetIO();

    // ImGui::SetCursorPosX((ImGui::GetWindowSize().x / 2.0f - 20.0f));
    // ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);

    // ImGui::PushItemWidth(10.0f);
    // ImGui::SmallButton(">");
    // if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left) &&
    //     Application::get().getTime().poused()) {
    //     Application::get().getTime().play();
    //     m_CameraPos = Application::get().getCamera().positionVec();
    //     m_CameraRotation = Application::get().getCamera().rotationVec();
    // }
    // ImGui::SameLine();
    // ImGui::SmallButton("||");
    // if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left) &&
    //     !Application::get().getTime().poused()) {
    //     Application::get().getTime().stop();
    //     Application::get().getCamera().setPosition(m_CameraPos);
    //     Application::get().getCamera().setRotation(m_CameraRotation);
    // }
    // ImGui::PopItemWidth();

    ImGuiStyle &style = ImGui::GetStyle();
    float minWinSizeX = style.WindowMinSize.x;
    style.WindowMinSize.x = 370.0f;
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    style.WindowMinSize.x = minWinSizeX;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    auto &colors = ImGui::GetStyle().Colors;
    const auto &buttonHovered = colors[ImGuiCol_ButtonHovered];
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
    const auto &buttonActive = colors[ImGuiCol_ButtonActive];
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

    ImGui::Begin("##toolbar", nullptr,
                 ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    float size = ImGui::GetWindowHeight() - 4.0f;
    Texture &icon = Application::get().getTime().poused() ? m_IconPlay : m_IconStop;
    ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));

    if (ImGui::ImageButton((ImTextureID)icon.id, ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0)) {
        if (Application::get().getTime().poused()) {
            Application::get().getTime().play();
            m_CameraPos = Application::get().getCamera().positionVec();
            m_CameraRotation = Application::get().getCamera().rotationQuat();
        } else {
            Application::get().getTime().stop();
            Application::get().getCamera().setPosition(m_CameraPos);
            Application::get().getCamera().setRotation(m_CameraRotation);
        }
    }

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(3);
    ImGui::End();

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

    // float ratio = m_ViewportSize.x / m_ViewportSize.y;
    // if (m_Ratio < ratio) {
    //     m_ViewportSize.x = m_ViewportSize.y * m_Ratio;
    // } else {
    //     m_ViewportSize.y = m_ViewportSize.x / m_Ratio;
    // }

    Application::get().getMousePicker().setRect(min, min + m_ViewportSize);

    ImGui::Image(reinterpret_cast<void *>(m_GColor.id), ImVec2{m_ViewportSize.x, m_ViewportSize.y}, ImVec2{0, 1},
                 ImVec2{1, 0});
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
            const char *path = static_cast<const char *>(payload->Data);
            auto &models = Application::get().getModels();
            if (!models.HasModel(path)) {
                std::filesystem::path modelSrc = std::filesystem::path("assets") / path;
                models.RegisterModel(path, ModelLoader::loadModel(modelSrc));
            }

            std::string name;
            size_t index = coordinator().GetEntities().size();
            do {
                name = std::string("entity") + std::to_string(index);
                index++;
            } while (coordinator().HasEntity(name));
            auto entity = coordinator().CreateEntity(name);

            coordinator().AddComponent<LocationComponent>(entity, LocationComponent(glm::vec3(0.0f)));
            coordinator().AddComponent<Render3DComponent>(entity, Render3DComponent(path, 1.0f));
            coordinator().AddComponent<TagComponent>(entity, TagComponent(name));

            if (models.Is<SkinnedModel>(path)) {
                coordinator().AddComponent<SkeletComponent>(entity, SkeletComponent());
            }
        }
        ImGui::EndDragDropTarget();
    }

    if (ImGui::BeginPopupContextWindow()) {
        if (ImGui::MenuItem("Add Box")) {
            std::string model = "primitive-box";
            auto &models = Application::get().getModels();
            if (!models.HasModel(model)) {
                models.RegisterModel(model, ModelFactory::createCube());
            }

            std::string name;
            size_t index = coordinator().GetEntities().size();
            do {
                name = std::string("entity") + std::to_string(index);
                index++;
            } while (coordinator().HasEntity(name));
            auto entity = coordinator().CreateEntity(name);

            coordinator().AddComponent<LocationComponent>(entity, LocationComponent(glm::vec3(0.0f)));
            coordinator().AddComponent<Render3DComponent>(entity, Render3DComponent(model, 1.0f));
            coordinator().AddComponent<TagComponent>(entity, TagComponent(name));
        }

        if (ImGui::MenuItem("Add Plane")) {
            std::string model = "primitive-plane";
            auto &models = Application::get().getModels();
            if (!models.HasModel(model)) {
                models.RegisterModel(model, ModelFactory::createPlane());
            }

            std::string name;
            size_t index = coordinator().GetEntities().size();
            do {
                name = std::string("entity") + std::to_string(index);
                index++;
            } while (coordinator().HasEntity(name));
            auto entity = coordinator().CreateEntity(name);
            coordinator().AddComponent<LocationComponent>(entity, LocationComponent(glm::vec3(0.0f)));
            coordinator().AddComponent<Render3DComponent>(entity, Render3DComponent(model, 1.0f));
            coordinator().AddComponent<TagComponent>(entity, TagComponent(name));
        }
        ImGui::EndPopup();
    }

    // if (ImGui::BeginDragDropTarget()) {
    //     if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
    //         const char *path = static_cast<const char *>(payload->Data);
    //     }
    //     ImGui::EndDragDropTarget();
    // }

    ImGui::End();
    ImGui::PopStyleVar();
}

void ImguiImpl::End() {
    ImGui::End();

    ImGuiIO &io = ImGui::GetIO();
    auto &window = Application::get().getWindow();
    io.DisplaySize = ImVec2((float)window.getWidth(), (float)window.getHeight());

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// void ImguiImpl::update() {
//     const auto &window = Application::get().getWindow();
//     const auto &time = Application::get().getTime();
//     auto &input = Application::get().getInput();

//     ImGuiIO &io = ImGui::GetIO();

//     int dWidth, dHeight;
//     window.getDrawableSize(dWidth, dHeight);
//     int width = window.getWidth();
//     int height = window.getHeight();

//     io.DisplaySize = ImVec2(float(width), float(height));
//     io.DisplayFramebufferScale =
//         ImVec2(width > 0 ? float(dWidth / width) : 0.f, height > 0 ? float(dHeight / height) : 0.f);

//     io.DeltaTime = time.getDeltaSeconds();
//     if (io.DeltaTime <= 0) {
//         io.DeltaTime = 0.00001f;
//     }

//     auto mousePos = input.GetMousePosition();
//     io.MousePos = ImVec2(mousePos.x, mousePos.y);

//     io.MouseDown[0] = input.IsMousePressed(MouseButton::Left);
//     io.MouseDown[1] = input.IsMousePressed(MouseButton::Right);
//     io.MouseDown[2] = input.IsMousePressed(MouseButton::Middle);

//     io.KeysDown[ImGuiKey_Backspace] = input.IsKeyPressed(KeyCode::Backspace);

//     io.AddInputCharactersUTF8(input.GetTextInput().data());
// }

// void ImguiImpl::render() {
//     ImGui::Render();
//     drawGui(ImGui::GetDrawData());
// }

// void ImguiImpl::drawGui(ImDrawData *drawData) {
//     auto &render = Application::get().getRender();

//     ImGuiIO &io = ImGui::GetIO();
//     int fbWidth = static_cast<int>(io.DisplaySize.x * io.DisplayFramebufferScale.x);
//     int fbHeight = static_cast<int>(io.DisplaySize.y * io.DisplayFramebufferScale.y);
//     if (fbWidth == 0 || fbHeight == 0) {
//         return;
//     }

//     drawData->ScaleClipRects(io.DisplayFramebufferScale);

//     float L = drawData->DisplayPos.x;
//     float R = drawData->DisplayPos.x + drawData->DisplaySize.x;
//     float T = drawData->DisplayPos.y;
//     float B = drawData->DisplayPos.y + drawData->DisplaySize.y;

// #if defined(GL_CLIP_ORIGIN)
//     if (!clip_origin_lower_left) {
//         float tmp = T;
//         T = B;
//         B = tmp;
//     } // Swap top and bottom if origin is upper left
// #endif

//     glm::mat4 model(1);
//     model = glm::translate(model, glm::vec3((R + L) / (L - R), (T + B) / (B - T), 0.0f));
//     model = glm::scale(model, glm::vec3(2.0f / (R - L), 2.0f / (T - B), -1.0f));

//     for (int n = 0; n < drawData->CmdListsCount; n++) {
//         const ImDrawList *cmdList = drawData->CmdLists[n];
//         const ImDrawIdx *idx_buffer_offset = nullptr;

//         std::vector<Mesh2D::Vertex> vertices;
//         for (const ImDrawVert &vertex : cmdList->VtxBuffer) {
//             float r = float(vertex.col & 255) / 255.0f;
//             float g = float((vertex.col >> 8) & 255) / 255.0f;
//             float b = float((vertex.col >> 16) & 255) / 255.0f;
//             float a = float((vertex.col >> 24) & 255) / 255.0f;

//             glm::vec2 position = glm::vec2(vertex.pos.x, vertex.pos.y);
//             glm::vec2 textCoord = glm::vec2(vertex.uv.x, vertex.uv.y);
//             glm::vec4 color = glm::vec4(r, g, b, a);

//             vertices.emplace_back(position, textCoord, color);
//         }

//         std::vector<uint32_t> indices(cmdList->IdxBuffer.begin(), cmdList->IdxBuffer.end());

//         for (size_t i = 0; i < cmdList->CmdBuffer.Size; i++) {
//             const ImDrawCmd *pcmd = &cmdList->CmdBuffer[i];
//             assert(pcmd->UserCallback == nullptr);

//             const auto *texture = reinterpret_cast<const Texture *>(pcmd->TextureId);

//             render.draw2D(vertices, indices, texture, model);
//         }
//     }
// }

} // namespace Engine
