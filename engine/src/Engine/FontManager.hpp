#pragma once

#include <memory>
#include <vector>

#include "FlatDictionary.hpp"
#include "Font.hpp"

namespace Engine {

class FontManager {
  public:
    void Add(const std::string &name, const std::string &path);

    std::shared_ptr<Font> Get(const std::string &name) const;

    bool Has(const std::string &name) const;

    const std::vector<std::string> &Keys();

  private:
    FlatDictionary<std::string, std::shared_ptr<Font>> m_Data;
};

} // namespace Engine
