#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace Engine {

class Mesh2D {
  public:
#pragma pack(push, 1)
    struct Vertex {
        glm::vec2 position = glm::vec2(0.0f);
        glm::vec2 textCoord = glm::vec2(0.0f);
        glm::vec4 color = glm::vec4(0.0f);

        Vertex() {}
        Vertex(glm::vec2 position, glm::vec2 textCoord, glm::vec4 color)
            : position(std::move(position)), textCoord(std::move(textCoord)), color(std::move(color)) {}
    };
#pragma pack(pop)
};

} // namespace Engine
