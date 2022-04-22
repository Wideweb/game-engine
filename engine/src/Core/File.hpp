#pragma once

#include <algorithm>
#include <array>
#include <string>

namespace Engine {

class File {
  public:
    static std::string read(const std::string &path);
    static std::string extension(const std::string &path);
    static std::string name(const std::string &path);

    template <size_t TSize>
    static bool isExtension(const std::string &path, const std::array<std::string, TSize> extensions) {
        return std::find(extensions.begin(), extensions.end(), File::extension(path)) != extensions.end();
    }
};

} // namespace Engine
