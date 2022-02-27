#pragma once

namespace Engine::Gfx {

unsigned int checkError_(const char *file, int line);

#define checkError() checkError_(__FILE__, __LINE__)

} // namespace Engine::Gfx