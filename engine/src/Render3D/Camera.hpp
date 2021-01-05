#pragma once

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>

namespace Engine {

class Camera {
  public:
    enum class Movement { FORWARD, BACKWARD, LEFT, RIGHT };
    enum class Projection { PERSPECTIVE, ORTHOGONAL };

    Camera(glm::vec3 position, glm::vec3 up, glm::vec3 direction);
    Camera();
    ~Camera();

    void move(Movement direction, float offset);
    void rotate(const glm::vec3 &offset);

    glm::mat4 viewMatrix() const;
    glm::mat4 projectionMatrix() const;
    glm::vec3 positionVec() const;

    void setSize(uint32_t width, uint32_t height);
    void setPerspective(float fieldOfView, float zNear, float zFar);
    void setOrthogonal(float zNear, float zFar);
    void setProjection(Projection mode);
    void setPosition(glm::vec3 position);
    void setRotation(glm::vec3 direction);

  private:
    uint32_t width;
    uint32_t height;

    glm::vec3 position;
    glm::vec3 up;
    glm::vec3 front;
    glm::vec3 rotation;

    Projection mode;
    glm::mat4 orthogonal;
    glm::mat4 perspective;
};

} // namespace Engine
