#include "Camera.hpp"
#include "Math.hpp"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

namespace Engine {

Camera::Camera() {}

Camera::Camera(glm::vec3 position, glm::vec3 up, glm::vec3 front)
    : position(std::move(position)), up(std::move(up)), front(std::move(front)) {}

Camera::~Camera() {}

glm::mat4 Camera::viewMatrix() const { return glm::lookAt(position, position + front, up); }

glm::mat4 Camera::projectionMatrix() const { return mode == Projection::PERSPECTIVE ? perspective : orthogonal; }

glm::mat4 Camera::orthogonalMatrix() const { return orthogonal; }

glm::vec3 Camera::positionVec() const { return position; }

glm::vec3 Camera::upVec() const { return up; }

void Camera::setSize(int width, int height) {
    this->width = width;
    this->height = height;

    setPerspective(fieldOfView, zNear, zFar);
}

void Camera::setPerspective(float fieldOfView, float zNear, float zFar) {
    this->fieldOfView = fieldOfView;
    this->zNear = zNear;
    this->zFar = zFar;
    perspective = glm::perspective(fieldOfView, static_cast<float>(width) / static_cast<float>(height), zNear, zFar);
}

void Camera::setOrthogonal(float zNear, float zFar) {
    orthogonal = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height), zNear, zFar);
}

void Camera::setProjection(Projection mode) { this->mode = mode; }

void Camera::setPosition(glm::vec3 position) { this->position = position; }

void Camera::setRotation(glm::vec3 rotation) {
    // if (rotation.x < -3.14f) {
    //     rotation.x += 6.28f;
    // } else if (rotation.x > 3.14f) {
    //     rotation.x -= 6.28f;
    // }

    // if (rotation.y < -3.14f) {
    //     rotation.y += 6.28f;
    // } else if (rotation.y > 3.14f) {
    //     rotation.y -= 6.28f;
    // }

    // if (rotation.z < -3.14f) {
    //     rotation.z += 6.28f;
    // } else if (rotation.z > 3.14f) {
    //     rotation.z -= 6.28f;
    // }

    this->rotation = rotation;
    this->front = glm::normalize((glm::quat(this->rotation) * glm::vec3(0.0f, 0.0f, -1.0f)));
}

void Camera::rotate(const glm::vec3 &offset) { this->setRotation(this->rotation + offset); }

void Camera::move(const glm::vec3 &offset) {
    position += front * offset.x;
    position += up * offset.y;
    position += glm::normalize(glm::cross(front, up)) * offset.z;
}

void Camera::inversePitch() { setRotation(this->rotation * glm::vec3(-1.0f, 1.0f, 1.0f)); }

glm::vec3 Camera::frontVec() const { return front; }

glm::vec3 Camera::rotationVec() const { return rotation; }

} // namespace Engine
