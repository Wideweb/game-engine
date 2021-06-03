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

  public:
    MousePicker(Input &input, Window &window, Camera &camera);

    void update();
    glm::vec3 ray();
};

} // namespace Engine
