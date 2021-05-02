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
        glm::vec3 position;
        glm::vec3 normal = glm::vec3(0.0f);
        glm::vec2 textCoord = glm::vec3(0.0f);
        VertexJointData joints;
        unsigned int jointsNumber;

        Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 textCoord)
            : position(std::move(position)), normal(std::move(normal)),
              textCoord(std::move(textCoord)), jointsNumber(0) {}
    };
#pragma pack(pop)

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    Material material;

    SkinnedMesh(const std::vector<Vertex> &vertices,
                std::vector<GLuint> &indices, const Material &material);

    SkinnedMesh(const SkinnedMesh &mesh);

    SkinnedMesh();
    ~SkinnedMesh();

    void draw(Shader &shader, const glm::mat4 &position,
              const std::vector<glm::mat4> &joints,
              unsigned int textureShift) const;

    void setUp();

  private:
    GLuint VAO, VBO, EBO, instanceVBO;
};

} // namespace Engine
