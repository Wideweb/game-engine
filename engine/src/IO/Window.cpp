#include "Window.hpp"
#include "SDLWindow.hpp"

namespace Engine {

Window *Window::create() { return new SDLWindow(); }

} // namespace Engine
