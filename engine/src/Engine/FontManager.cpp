#include "FontManager.hpp"

namespace Engine {

void FontManager::Add(const std::string &name, const std::string &path) {
    m_Data.add(name, std::make_shared<Font>(path));
}

std::shared_ptr<Font> FontManager::Get(const std::string &name) const {
    assert(m_Data.hasKey(name) && "no font.");
    return m_Data[name];
}

bool FontManager::Has(const std::string &name) const { return m_Data.hasKey(name); }

const std::vector<std::string> &FontManager::Keys() { return m_Data.keys(); }

} // namespace Engine
