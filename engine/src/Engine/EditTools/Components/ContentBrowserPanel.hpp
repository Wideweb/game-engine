#pragma once

#include "BaseView.hpp"
#include "Entity.hpp"
#include "GameObjectModel.hpp"
#include "Texture.hpp"

#include <filesystem>
#include <string>
#include <memory>

namespace Engine {

class ContentBrowserPanel : public BaseView {
  private:
    std::string m_AssetPath;
    std::filesystem::path m_CurrentDirectory;

    std::unique_ptr<Texture> m_DirectoryIcon, m_FileIcon;

  public:
    ContentBrowserPanel(std::string directory);

    void onDraw(int x, int y) override;
};

} // namespace Engine
