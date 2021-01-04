#include "Input.hpp"
#include "SDLInput.hpp"

namespace Engine {

Input *Input::create() { return new SDLInput(); }

} // namespace Engine
