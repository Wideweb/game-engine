#include "CameraController.hpp"

#include "Easing.hpp"

#include <glm/gtx/transform.hpp>

namespace Engine {

CameraController::CameraController(Camera &camera) : m_Camera(camera) {}

void CameraController::move(const glm::vec3 &delta, double time) {
    m_AnimationTime = time > 0.0 ? time : 0.001;
    m_ElapsedTime = 0.0;
    m_Running = true;
    m_Animation = Animation::Move;
    m_PositionEnd = delta;
    m_PositionBegin = glm::vec3(0.0f);
    m_PrevDelta = glm::vec3(0.0f);

    update(0.016);
}

void CameraController::moveTo(const glm::vec3 &position, double time) {
    m_AnimationTime = time > 0.0 ? time : 0.001;
    m_ElapsedTime = 0.0;
    m_Running = true;
    m_Animation = Animation::MoveTo;
    m_PositionEnd = position;
    m_PositionBegin = m_Camera.positionVec();

    update(0.016);
}

void CameraController::rotateTo(const glm::quat &rotation, double time) {
    m_AnimationTime = time > 0.0 ? time : 0.001;
    m_ElapsedTime = 0.0;
    m_Running = true;
    m_Animation = Animation::Rotation;
    m_RotationEnd = rotation;
    m_RotationBegin = m_Camera.rotationQuat();

    update(0.016);
}

void CameraController::spin(const glm::vec3 &pivot, const glm::quat &rotation, double time) {
    m_AnimationTime = time > 0.0 ? time : 0.001;
    m_ElapsedTime = 0.0;
    m_Running = true;
    m_Animation = Animation::Spin;
    m_RotationEnd = rotation;
    m_RotationBegin = m_Camera.rotationQuat();
    m_Pivot = pivot;

    update(0.016);
}

void CameraController::update(double deltaTime) {
    if (!m_Running) {
        return;
    }

    m_ElapsedTime += deltaTime;

    if (m_ElapsedTime >= m_AnimationTime) {
        m_ElapsedTime = m_AnimationTime;
        m_Running = false;
    }

    float factor = static_cast<float>(easeOutQuart(m_ElapsedTime / m_AnimationTime));

    if (m_Animation == Animation::MoveTo) {
        glm::vec3 position = glm::mix(m_PositionBegin, m_PositionEnd, factor);
        m_Camera.setPosition(position);
    }

    if (m_Animation == Animation::Move) {
        glm::vec3 delta = glm::mix(m_PositionBegin, m_PositionEnd, factor);
        m_Camera.move(delta - m_PrevDelta);
        m_PrevDelta = delta;
    }

    if (m_Animation == Animation::Rotation) {
        glm::quat rotation = glm::slerp(m_RotationBegin, m_RotationEnd, factor);
        m_Camera.setRotation(rotation);
    }

    if (m_Animation == Animation::Spin) {
        glm::quat rotation = glm::slerp(m_RotationBegin, m_RotationEnd, factor);
        glm::quat deltaRotation = rotation * glm::inverse(m_Camera.rotationQuat());
        glm::vec3 position = m_Pivot + deltaRotation * (m_Camera.positionVec() - m_Pivot);

        m_Camera.setPosition(position);
        m_Camera.setRotation(rotation);
    }
}

} // namespace Engine