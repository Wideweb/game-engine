#pragma once

#include "Entity.hpp"

#include <utility>

namespace Engine {

class SkeletComponent {
  public:
    std::string animation;

    SkeletComponent() {}
    SkeletComponent(std::string animation) : animation(std::move(animation)) {}
};

} // namespace Engine