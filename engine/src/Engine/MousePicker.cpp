#include "MousePicker.hpp"

#include <glm/gtx/transform.hpp>

namespace Engine {

MousePicker::MousePicker(Input &input, Window &window, Camera &camera)
    : m_Input(input), m_Window(window), m_Camera(camera) {}

void MousePicker::update() {
    glm::vec2 mouse = m_Input.GetMousePosition();
    mouse.x = mouse.x * 2.0f / m_Window.getWidth() - 1.0f;
    mouse.y = 1.0f - mouse.y * 2.0f / m_Window.getHeight();

    glm::vec4 coords = glm::vec4(mouse, -1.0f, 1.0f);
    coords = glm::inverse(m_Camera.projectionMatrix()) * coords;
    coords = glm::vec4(glm::vec2(coords), -1.0, 0.0f);
    coords = glm::inverse(m_Camera.viewMatrix()) * coords;

    m_Ray = glm::normalize(glm::vec3(coords));
}

glm::vec3 MousePicker::ray() { return m_Ray; }

} // namespace Engine
