#include "SDLInput.hpp"

#include <iostream>

namespace Engine {

static SDL_Scancode getSDLScancode(KeyCode code) {
    switch (code) {
    case KeyCode::Backspace:
        return SDL_SCANCODE_BACKSPACE;
    case KeyCode::Space:
        return SDL_SCANCODE_SPACE;
    case KeyCode::A:
        return SDL_SCANCODE_A;
    case KeyCode::D:
        return SDL_SCANCODE_D;
    case KeyCode::S:
        return SDL_SCANCODE_S;
    case KeyCode::W:
        return SDL_SCANCODE_W;
    case KeyCode::F:
        return SDL_SCANCODE_F;
    case KeyCode::Q:
        return SDL_SCANCODE_Q;
    case KeyCode::E:
        return SDL_SCANCODE_E;
    case KeyCode::Escape:
        return SDL_SCANCODE_ESCAPE;
    default:
        return SDL_SCANCODE_UNKNOWN;
    }
}

static int getSDLButton(MouseButton code) {
    switch (code) {
    case MouseButton::Left:
        return SDL_BUTTON_LEFT;
    case MouseButton::Right:
        return SDL_BUTTON_RIGHT;
    case MouseButton::Middle:
        return SDL_BUTTON_MIDDLE;
    default:
        return 0;
    }
}

void SDLInput::update() {}

bool SDLInput::IsKeyPressed(KeyCode key) {
    const Uint8 *keystates = SDL_GetKeyboardState(NULL);
    return keystates[getSDLScancode(key)];
}

bool SDLInput::IsMousePressed(MouseButton button) {
    return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(getSDLButton(button));
}

glm::vec2 SDLInput::GetMousePosition() {
    int x, y;
    SDL_GetMouseState(&x, &y);
    return glm::vec2(float(x), float(y));
}

void SDLInput::SetTextInput(const std::string &input) { m_TextInput = input; }

std::string SDLInput::GetTextInput() { return m_TextInput; }

} // namespace Engine
