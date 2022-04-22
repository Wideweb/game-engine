#pragma once

#include <glm/vec3.hpp>
#include <string>

namespace Engine {

class Text2DComponent {
  public:
    enum class VerticalAlignment { Bottom, Middle, Top };
    enum class HorizontalAlignment { Left, Middle, Right };

    std::string font, text;
    glm::vec3 color = glm::vec3(0.0f);
    VerticalAlignment verticalAlignment = VerticalAlignment::Middle;
    HorizontalAlignment horizontalAlignment = HorizontalAlignment::Middle;

    bool instanced = false;
    bool updated = false;

    Text2DComponent() {}
    Text2DComponent(std::string text, std::string font) : text(text), font(font) {}
};

} // namespace Engine