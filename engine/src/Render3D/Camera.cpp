#include "Camera.hpp"
#include "Math.hpp"

#include <glm/gtx/transform.hpp>

#include <cmath>
#include <iostream>

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

glm::vec2 Camera::size() const { return {this->width, this->height}; }

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

void Camera::setRotation(glm::quat rotation) {
    this->rotation = rotation;
    this->front = glm::normalize((this->rotation * glm::vec3(0.0f, 0.0f, -1.0f)));
    // std::cout << this->front.x << " " << this->front.y << " " << this->front.z << std::endl;
}

void Camera::move(const glm::vec3 &offset) {
    position += front * offset.x;
    position += up * offset.y;
    position += glm::normalize(glm::cross(front, up)) * offset.z;
}

void Camera::inversePitch() {
    this->front = glm::reflect(this->front, this->up);

    // if (std::abs(glm::dot(this->front, newDir)) < 0.1f) {
    //     return;
    // }

    // glm::vec3 half = glm::normalize(this->front + newDir);
    // glm::quat deltaRotation = glm::quat(glm::dot(this->front, half), glm::cross(this->front, half));
    // setRotation(deltaRotation * this->rotation);

    // glm::vec3 right = glm::normalize(glm::cross(front, up));
    // glm::quat deltaRotation = glm::angleAxis(-angles.x, right);
    // setRotation(glm::quat(glm::eulerAngles(this->rotation) * glm::vec3(-1.0f, 1.0f, -1.0f)));
}

glm::vec3 Camera::frontVec() const { return front; }

glm::quat Camera::rotationQuat() const { return this->rotation; }

} // namespace Engine
