#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "Shader.hpp"
#include "Texture.hpp"

namespace Engine {

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal = glm::vec3(0.0f);
    glm::vec2 textCoord = glm::vec3(0.0f);

    Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 textCoord)
        : position(std::move(position)), normal(std::move(normal)),
          textCoord(std::move(textCoord)) {}
};

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

    void draw(Shader &shader) const;

  private:
    GLuint VAO, VBO, EBO;

    void setUp();
};

} // namespace Engine
