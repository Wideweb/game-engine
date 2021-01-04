#pragma once

#include "Texture.hpp"

#include <memory>
#include <string>
#include <unordered_map>

namespace Engine {

class TextureManager {
  private:
    std::unordered_map<std::string, std::shared_ptr<Texture>> m_Map;

  public:
    void load(const std::string &name, const std::string &path);
    void add(const std::string &name, Texture *texture);
    std::shared_ptr<Texture> get(const std::string &name);
};

} // namespace Engine
