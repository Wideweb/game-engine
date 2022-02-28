#pragma once

#include "Shader.hpp"
#include "Texture.hpp"

#include <array>
#include <glm/glm.hpp>
#include <memory>

namespace Engine {

class CubeMap {
  private:
    Texture m_CubeMapTexture;
    std::array<glm::mat4, 6> m_Transforms;
    float m_FarPlane;
    glm::vec3 m_Position;
    glm::mat4 m_Projection;

  public:
    CubeMap();
    CubeMap(int width, int height, float farPlane, glm::vec3 position);
    ~CubeMap();

    void bind(Shader &shader);
    void setPosition(const glm::vec3 &position);

    const Texture &texture() const { return m_CubeMapTexture; }
    float farPlane() const { return m_FarPlane; }

  private:
    void updateTransforms();
};

} // namespace Engine