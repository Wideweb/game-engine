#pragma once

#include "BaseView.hpp"
#include "Entity.hpp"
#include "GameObjectModel.hpp"
#include "Texture.hpp"

#include <filesystem>
#include <memory>
#include <string>

namespace Engine {

class ContentBrowserPanel : public BaseView {
  private:
    std::string m_AssetPath;
    std::filesystem::path m_CurrentDirectory;

    Texture m_DirectoryIcon, m_FileIcon;

  public:
    ContentBrowserPanel(std::string directory);

    void onDraw() override;
};

} // namespace Engine
