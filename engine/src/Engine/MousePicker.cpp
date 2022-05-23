#include "MousePicker.hpp"

#include <glm/gtx/transform.hpp>

namespace Engine {

MousePicker::MousePicker(Input &input, Window &window, Camera &camera)
    : m_Input(input), m_Window(window), m_Camera(camera) {}

void MousePicker::update() {
    glm::vec2 scale = m_Window.getSize() / (m_Rect[1] - m_Rect[0]);
    glm::vec2 mouse = getPos() * scale;

    mouse.x = mouse.x * 2.0f / m_Window.getWidth() - 1.0f;
    mouse.y = 1.0f - mouse.y * 2.0f / m_Window.getHeight();

    glm::vec4 coords = glm::vec4(mouse, -1.0f, 1.0f);
    coords = glm::inverse(m_Camera.projectionMatrix()) * coords;
    coords = glm::vec4(glm::vec2(coords), -1.0, 0.0f);
    coords = glm::inverse(m_Camera.viewMatrix()) * coords;

    m_Ray = glm::normalize(glm::vec3(coords));
}

glm::vec3 MousePicker::ray() { return m_Ray; }

void MousePicker::setRect(glm::vec2 min, glm::vec2 max) {
    m_Rect[0] = min;
    m_Rect[1] = max;
}

glm::vec2 MousePicker::getPos() { return m_Input.GetMousePosition() - m_Rect[0]; }

} // namespace Engine
