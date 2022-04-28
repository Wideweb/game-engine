#include "ContentBrowserPanel.hpp"

#include "imgui/imgui.h"

#include "ImGuiWidgets.hpp"
#include "TextureLoader.hpp"

namespace Engine {

ContentBrowserPanel::ContentBrowserPanel(std::string path)
    : m_AssetPath(path), m_CurrentDirectory(std::filesystem::path(path)) {
    m_DirectoryIcon = TextureLoader::loadTexture("assets/textures/icons/directory.png");
    m_FileIcon = TextureLoader::loadTexture("assets/textures/icons/file.png");
}

void ContentBrowserPanel::onDraw() {
    ImGui::Begin("Content Browser");

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    auto &colors = ImGui::GetStyle().Colors;
    const auto &buttonHovered = colors[ImGuiCol_ButtonHovered];
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
    const auto &buttonActive = colors[ImGuiCol_ButtonActive];
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

    bool root = true;
    for (auto it = m_CurrentDirectory.begin(); it != m_CurrentDirectory.end(); ++it) {
        if (it != m_CurrentDirectory.begin()) {
            ImGui::SameLine();
            ImGuiWidgets::PaddingLeft(-5.0f);
            ImGui::Text(">");
            ImGui::SameLine();
            ImGuiWidgets::PaddingLeft(-5.0f);
        }

        if (ImGui::Button(it->c_str())) {
            std::filesystem::path atPath = std::filesystem::path(m_AssetPath);
            for (auto at = std::next(m_CurrentDirectory.begin()); at != std::next(it); ++at) {
                atPath /= (*at);
            }
            m_CurrentDirectory = atPath;
            break;
        }
    }

    ImGui::PopStyleColor(3);

    ImGui::BeginChild("Content Browser Files");
    static float padding = 16.0f;
    static float thumbnailSize = 64.0f;
    float cellSize = thumbnailSize + padding;

    float panelWidth = ImGui::GetContentRegionAvail().x;
    int columnCount = (int)(panelWidth / cellSize);
    if (columnCount < 1)
        columnCount = 1;

    ImGui::Columns(columnCount, 0, false);

    for (auto &directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory)) {
        const auto &path = directoryEntry.path();
        auto relativePath = std::filesystem::relative(path, m_AssetPath);
        std::string filenameString = relativePath.filename().string();

        if (filenameString[0] == '.') {
            continue;
        }

        ImGui::PushID(filenameString.c_str());
        uint32_t icon = directoryEntry.is_directory() ? m_DirectoryIcon.id : m_FileIcon.id;
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
        ImGui::ImageButton(reinterpret_cast<ImTextureID>(icon), {thumbnailSize, thumbnailSize}, {0, 0}, {1, 1});

        if (ImGui::BeginDragDropSource()) {
            const char *itemPath = relativePath.c_str();
            ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", relativePath.c_str(),
                                      (strlen(itemPath) + 1) * sizeof(char));
            ImGui::EndDragDropSource();
        }

        ImGui::PopStyleColor();
        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
            if (directoryEntry.is_directory())
                m_CurrentDirectory /= path.filename();
        }
        ImGui::TextWrapped(filenameString.c_str());

        ImGui::NextColumn();

        ImGui::PopID();
    }

    ImGui::Columns(1);
    ImGui::EndChild();

    // ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
    // ImGui::SliderFloat("Padding", &padding, 0, 32);

    // TODO: status bar
    ImGui::End();
}

} // namespace Engine
