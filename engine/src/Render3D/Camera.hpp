#pragma once

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>

namespace Engine {

class Camera {
  public:
    enum class Projection { PERSPECTIVE, ORTHOGONAL };

    Camera(glm::vec3 position, glm::vec3 up, glm::vec3 front);
    Camera();
    ~Camera();

    void move(const glm::vec3 &offset);
    void rotate(const glm::vec3 &offset);

    glm::mat4 viewMatrix() const;
    glm::mat4 projectionMatrix() const;
    glm::mat4 orthogonalMatrix() const;
    glm::vec3 positionVec() const;
    glm::vec3 upVec() const;
    glm::vec3 frontVec() const;
    glm::vec3 rotationVec() const;
    glm::vec2 size() const;

    void setSize(int width, int height);
    void setPerspective(float fieldOfView, float zNear, float zFar);
    void setOrthogonal(float zNear, float zFar);
    void setProjection(Projection mode);
    void setPosition(glm::vec3 position);
    void setRotation(glm::vec3 direction);
    void inversePitch();

  private:
    int width;
    int height;

    glm::vec3 position;
    glm::vec3 up;
    glm::vec3 front;
    glm::vec3 rotation;

    Projection mode;
    glm::mat4 orthogonal;
    glm::mat4 perspective;

    float fieldOfView, zNear, zFar;
};

} // namespace Engine
