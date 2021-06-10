#pragma once

#include <memory>
#include <string>

namespace Engine {

class TagComponent {
  public:
    std::string tag;

    TagComponent() {}
    TagComponent(std::string tag) : tag(std::move(tag)) {}
};

} // namespace Engine