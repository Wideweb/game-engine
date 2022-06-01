#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "Material.hpp"
#include "Shader.hpp"

namespace Engine {

class SkinnedMesh {
  public:
#pragma pack(push, 1)
    struct VertexJointData {
        unsigned int ids[8];
        float weights[8];
    };
#pragma pack(pop)

#pragma pack(push, 1)
    struct Vertex {
        glm::vec3 position = glm::vec3(0.0f);
        glm::vec3 normal = glm::vec3(0.0f);
        glm::vec2 textCoord = glm::vec3(0.0f);
        glm::vec3 tangent = glm::vec3(0.0f);
        glm::vec3 bitangent = glm::vec3(0.0f);
        glm::vec3 color = glm::vec4(0.0f);
        VertexJointData joints;
        unsigned int jointsNumber;

        Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 textCoord, glm::vec3 tangent, glm::vec3 bitangent,
               glm::vec3 color)
            : position(std::move(position)), normal(std::move(normal)), textCoord(std::move(textCoord)),
              tangent(std::move(tangent)), bitangent(std::move(bitangent)), color(color), jointsNumber(0) {}
    };
#pragma pack(pop)

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    Material material;

    SkinnedMesh(const std::vector<Vertex> &vertices, std::vector<unsigned int> &indices);

    SkinnedMesh(const SkinnedMesh &mesh);

    SkinnedMesh();
    ~SkinnedMesh();

    void draw(Shader &shader, const glm::mat4 &position, const std::vector<glm::mat4> &joints, uint32_t id) const;

    void setUp();
    void update();

  private:
    unsigned int VAO, VBO, EBO, instanceVBO, idVBO;
};

} // namespace Engine
