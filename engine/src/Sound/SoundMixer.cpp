#include "SoundMixer.hpp"
#include "SDLSoundMixer.hpp"

namespace Engine {

SoundMixer *SoundMixer::create() { return new SDLSoundMixer(); }

} // namespace Engine
