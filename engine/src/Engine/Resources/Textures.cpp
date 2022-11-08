#include "Textures.hpp"
#include "TextureLoader.hpp"

#include <stdexcept>

namespace Engine {

void Textures::init() { }

void Textures::load(const std::string &name, const std::string &path) {
    if (m_Map.find(name) != m_Map.end()) {
        throw std::invalid_argument("texture name is not unique.");
    }
    m_Map.insert({name, TextureLoader::loadTexture(path)});
}

void Textures::add(const std::string &name, const Texture &texture) { m_Map[name] = texture; }

Texture* Textures::get(const std::string &name) { return &m_Map[name]; }

Texture* Textures::loadIfNotExist(const std::string &name) {
    if (m_Map.find(name) == m_Map.end()) {
        load(name, name);
    }
    return get(name);
}

} // namespace Engine
