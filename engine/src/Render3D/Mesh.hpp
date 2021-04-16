#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "Shader.hpp"
#include "Texture.hpp"

namespace Engine {

#pragma pack(push, 1)
struct VertexBoneData {
    unsigned int ids[4];
    float weights[4];
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Vertex {
    glm::vec3 position;
    glm::vec3 normal = glm::vec3(0.0f);
    glm::vec2 textCoord = glm::vec3(0.0f);
    VertexBoneData bones;
    unsigned int bonesNumber;

    Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 textCoord)
        : position(std::move(position)), normal(std::move(normal)),
          textCoord(std::move(textCoord)), bonesNumber(0) {}
};
#pragma pack(pop)

struct Material {
    std::shared_ptr<Texture> diffuseMap;
    std::shared_ptr<Texture> specularMap;
    std::shared_ptr<Texture> normalMap;
    float specular = 1.0f;
    float shininess = 128.0f;
};

class Mesh {
  public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    Material material;

    Mesh(const std::vector<Vertex> &vertices, std::vector<GLuint> &indices,
         const Material &material);

    Mesh(const Mesh &mesh);

    Mesh();
    ~Mesh();

    void setInstances(const std::vector<glm::mat4> &positions) const;
    void updateInstances(size_t from, size_t to,
                         const std::vector<glm::mat4> &positions) const;

    void draw(Shader &shader, size_t instanceCount,
              unsigned int textureShift) const;

    void setUp();

  private:
    GLuint VAO, VBO, EBO, instanceVBO;
};

} // namespace Engine
