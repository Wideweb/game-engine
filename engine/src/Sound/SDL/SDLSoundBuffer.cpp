#include "SDLSoundBuffer.hpp"

#include <stdexcept>

namespace Engine {

SDLSoundBuffer::SDLSoundBuffer(const std::string &path,
                               SDL_AudioDeviceID device,
                               SDL_AudioSpec deviceSpec)
    : m_Device(device) {
    SDL_RWops *file = SDL_RWFromFile(path.data(), "rb");
    if (file == nullptr) {
        throw std::runtime_error(std::string("can't open audio file: ") +
                                 path.data());
    }

    SDL_AudioSpec fileSpec;

    if (nullptr == SDL_LoadWAV_RW(file, 1, &fileSpec, &m_Buffer, &m_Length)) {
        throw std::runtime_error(std::string("can't load wav: ") + path.data());
    }

    if (fileSpec.channels != deviceSpec.channels ||
        fileSpec.format != deviceSpec.format ||
        fileSpec.freq != deviceSpec.freq) {
        throw std::runtime_error(std::string("wrong wav spec: ") + path.data());
    }
}

SDLSoundBuffer::~SDLSoundBuffer() {
    SDL_FreeWAV(m_Buffer);
    m_Buffer = nullptr;
    m_Length = 0;
}

void SDLSoundBuffer::play(const Properties prop, float volume) {
    SDL_LockAudioDevice(m_Device);

    m_Index = 0;
    m_Playing = true;
    m_Looped = (prop == Properties::Looped);
    m_Volume = volume;

    SDL_UnlockAudioDevice(m_Device);
}

void *SDLSoundBuffer::getDevice() { return &m_Device; }

} // namespace Engine
