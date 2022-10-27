#pragma once

#include "Texture.hpp"

#include <memory>
#include <string>
#include <unordered_map>

namespace Engine {

class Textures {
  private:
    std::unordered_map<std::string, Texture> m_Map;

  public:
    void init(); 
    void load(const std::string &name, const std::string &path);
    void add(const std::string &name, const Texture &texture);
    Texture* get(const std::string &name);
    Texture* loadIfNotExist(const std::string &name);
};

} // namespace Engine
