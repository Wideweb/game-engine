#include "Camera.hpp"
#include "Math.hpp"

#include <glm/gtx/transform.hpp>

namespace Engine {

Camera::Camera() {}

Camera::Camera(glm::vec3 position, glm::vec3 up, glm::vec3 front)
    : position(std::move(position)), up(std::move(up)),
      front(std::move(front)) {}

Camera::~Camera() {}

glm::mat4 Camera::viewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::projectionMatrix() const {
    return mode == Projection::PERSPECTIVE ? perspective : orthogonal;
}

glm::vec3 Camera::positionVec() const { return position; }

glm::vec3 Camera::upVec() const { return up; }

void Camera::setSize(uint32_t width, uint32_t height) {
    this->width = width;
    this->height = height;
}

void Camera::setPerspective(float fieldOfView, float zNear, float zFar) {
    perspective = glm::perspective(
        fieldOfView, static_cast<float>(width) / static_cast<float>(height),
        zNear, zFar);
}

void Camera::setOrthogonal(float zNear, float zFar) {
    orthogonal = glm::ortho(0.0f, static_cast<float>(width), 0.0f,
                            static_cast<float>(height), zNear, zFar);
}

void Camera::setProjection(Projection mode) { this->mode = mode; }

void Camera::setPosition(glm::vec3 position) { this->position = position; }

void Camera::setRotation(glm::vec3 rotation) {
    this->rotation = rotation;
    this->front = Math::getDirection(rotation);
}

void Camera::rotate(const glm::vec3 &offset) {
    rotation += offset;
    this->front = Math::getDirection(rotation);
}

void Camera::move(const glm::vec3 &offset) {
    position += front * offset.x;
    position += up * offset.y;
    position += glm::normalize(glm::cross(front, up)) * offset.z;
}

void Camera::inversePitch() { rotate(glm::vec3(rotation.x * -2.0, 0.0, 0.0)); }

} // namespace Engine
