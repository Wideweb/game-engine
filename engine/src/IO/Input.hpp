#pragma once

#include <glm/vec2.hpp>
#include <string>
#include <vector>

namespace Engine {

enum class KeyCode {
    None = 0,
    Backspace = 8,
    Shift = 16,
    Space = 32,
    A = 65,
    D = 68,
    E = 69,
    Q = 81,
    S = 83,
    W = 87,
    F = 70,
    X = 88,
    Y = 89,
    Z = 90,
    Escape = 256,
    LeftControl = 17,
    LeftMeta = 91,
};

enum class MouseButton { None = 0, Left = 1, Right = 2, Middle = 3 };

class Input {
  public:
    virtual ~Input() = default;
    virtual void update() {}
    virtual bool IsKeyPressed(KeyCode key) = 0;
    virtual bool IsAnyKeyPressed(const std::vector<KeyCode> keys) = 0;
    virtual bool IsMousePressed(MouseButton button) = 0;
    virtual glm::vec2 GetMousePosition() = 0;
    virtual std::string GetTextInput() = 0;
    virtual void SetTextInput(const std::string &) = 0;

    static Input *create();
};

} // namespace Engine
