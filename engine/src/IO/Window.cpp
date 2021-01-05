#include "Window.hpp"
#include "SDLWindow.hpp"

namespace Engine {

Window *Window::create(const WindowProps &props) { return new SDLWindow(props); }

} // namespace Engine
