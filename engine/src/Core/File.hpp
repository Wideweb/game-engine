#pragma once

#include <string>

namespace Engine {

class File {
  public:
    static std::string read(const std::string &path);
};

} // namespace Engine
