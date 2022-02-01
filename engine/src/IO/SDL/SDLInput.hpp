#pragma once

#include "Input.hpp"

#include <SDL2/SDL.h>
#include <string>

namespace Engine {

class SDLInput : public Input {
  private:
    std::string m_TextInput;

  public:
    virtual void update() override;
    virtual bool IsKeyPressed(KeyCode key) override;
    virtual bool IsAnyKeyPressed(const std::vector<KeyCode> keys) override;
    virtual bool IsMousePressed(MouseButton button) override;
    virtual glm::vec2 GetMousePosition() override;
    virtual std::string GetTextInput() override;
    virtual void SetTextInput(const std::string &) override;
};

} // namespace Engine
