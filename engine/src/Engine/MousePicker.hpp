#pragma once

#include "Camera.hpp"
#include "Input.hpp"
#include "Window.hpp"

#include <glm/glm.hpp>

namespace Engine {

class MousePicker {
  private:
    glm::vec3 m_Ray;

    Input &m_Input;
    Window &m_Window;
    Camera &m_Camera;
    glm::vec2 m_Rect[2] = {{0.0f, 0.0f}, {0.0f, 0.0f}};

  public:
    MousePicker(Input &input, Window &window, Camera &camera);

    void update();
    glm::vec3 ray();
    void setRect(glm::vec2 min, glm::vec2 max);
    glm::vec2 getPos();
};

} // namespace Engine
