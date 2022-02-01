#pragma once

#include "Camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Engine {

class CameraController {
  private:
    enum class Animation { None, MoveTo, Move, Rotation, Spin };

    Camera &m_Camera;
    double m_ElapsedTime = 0.0, m_AnimationTime = 0.0;
    bool m_Running = false;
    Animation m_Animation = Animation::None;
    glm::vec3 m_PositionBegin, m_PositionEnd;
    glm::quat m_RotationBegin, m_RotationEnd;
    glm::vec3 m_PrevDelta;
    glm::vec3 m_Pivot;

  public:
    CameraController(Camera &camera);

    void move(const glm::vec3 &delta, double time);
    void moveTo(const glm::vec3 &position, double time);
    void rotateTo(const glm::quat &rotation, double time);
    void spin(const glm::vec3 &pivot, const glm::quat &rotation, double time);

    void update(double deltaTime);
};

} // namespace Engine
