#include "TextureManager.hpp"

namespace Engine {

void TextureManager::load(const std::string &name, const std::string &path) {
    m_Map[name] = std::make_shared<Texture>(path);
}

void TextureManager::add(const std::string &name, Texture *texture) {
    m_Map[name].reset(texture);
}

std::shared_ptr<Texture> TextureManager::get(const std::string &name) {
    return m_Map[name];
}

} // namespace Engine
