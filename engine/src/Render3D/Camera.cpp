#include "Camera.hpp"
#include "Math.hpp"

#include <glm/gtx/transform.hpp>

namespace Engine {

Camera::Camera() {}

Camera::Camera(const glm::vec3 &position, const glm::vec3 &up,
               const glm::vec3 &front)
    : position(position), up(up), front(front) {}

Camera::~Camera() {}

glm::mat4 Camera::viewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::projectionMatrix() const {
    return mode == Projection::PERSPECTIVE ? perspective : orthogonal;
}

glm::vec3 Camera::positionVec() const { return position; }

void Camera::setSize(uint32_t width, uint32_t height) {
    this->width = width;
    this->height = height;
}

void Camera::setPerspective(float fieldOfView, float zNear, float zFar) {
    perspective = glm::perspective(fieldOfView, (float)width / (float)height,
                                   zNear, zFar);
}

void Camera::setOrthogonal(float zNear, float zFar) {
    orthogonal =
        glm::ortho(0.0f, (float)width, 0.0f, (float)height, zNear, zFar);
}

void Camera::setProjection(Projection mode) { this->mode = mode; }

void Camera::setPosition(const glm::vec3 position) {
    this->position = position;
}

void Camera::setRotation(const glm::vec3 rotation) {
    this->rotation = rotation;
    this->front = Math::getDirection(rotation);
}

void Camera::rotate(const glm::vec3 &offset) {
    rotation += offset;
    this->front = Math::getDirection(rotation);
}

void Camera::move(Movement direction, float offset) {
    if (direction == Movement::FORWARD)
        position += front * offset;
    if (direction == Movement::BACKWARD)
        position -= front * offset;
    if (direction == Movement::LEFT)
        position -= glm::normalize(glm::cross(front, up)) * offset;
    if (direction == Movement::RIGHT)
        position += glm::normalize(glm::cross(front, up)) * offset;
}

} // namespace Engine
