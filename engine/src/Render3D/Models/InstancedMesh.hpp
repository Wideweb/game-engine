#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "Material.hpp"
#include "Shader.hpp"

namespace Engine {

class InstancedMesh {
  public:
#pragma pack(push, 1)
    struct Vertex {
        glm::vec3 position = glm::vec3(0.0f);
        glm::vec3 normal = glm::vec3(0.0f);
        glm::vec2 textCoord = glm::vec3(0.0f);
        glm::vec3 tangent = glm::vec3(0.0f);
        glm::vec3 bitangent = glm::vec3(0.0f);
        glm::vec3 color = glm::vec3(0.0f);

        Vertex() {}
        Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 textCoord, glm::vec3 tangent, glm::vec3 bitangent,
               glm::vec3 color)
            : position(std::move(position)), normal(std::move(normal)), textCoord(std::move(textCoord)),
              tangent(std::move(tangent)), bitangent(std::move(bitangent)), color(std::move(color)) {}
    };
#pragma pack(pop)

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    Material material;
    bool hasMaterial = false;

    InstancedMesh(const std::vector<Vertex> &vertices, std::vector<GLuint> &indices, const Material &material);

    InstancedMesh(const InstancedMesh &mesh);

    InstancedMesh();
    ~InstancedMesh();

    void setInstances(const std::vector<glm::mat4> &positions) const;
    void updateInstances(size_t from, size_t to, const std::vector<glm::mat4> &positions) const;

    void draw(Shader &shader, size_t instanceCount, unsigned int textureShift) const;

    void setUp();

  private:
    GLuint VAO, VBO, EBO, instanceVBO;
};

} // namespace Engine
