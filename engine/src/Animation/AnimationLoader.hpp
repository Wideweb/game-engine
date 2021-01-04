#pragma once

#include "AnimationScene.hpp"

#include <string>

namespace Engine {

class AnimationLoader {
  public:
    static AnimationScene load(const std::string &path);
};

} // namespace Engine