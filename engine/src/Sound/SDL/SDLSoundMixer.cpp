#include "SDLSoundMixer.hpp"

#include <iostream>
#include <stdexcept>

namespace Engine {

SDLSoundMixer::SDLSoundMixer() {
    m_DeviceSpec.freq = 48000;
    m_DeviceSpec.format = AUDIO_S16LSB;
    m_DeviceSpec.channels = 2;
    m_DeviceSpec.samples = 1024;
    m_DeviceSpec.callback = SDLSoundMixer::audioCallback;
    m_DeviceSpec.userdata = this;

    const char *defaultDeviceName = SDL_GetAudioDeviceName(0, SDL_FALSE);
    m_Device = SDL_OpenAudioDevice(defaultDeviceName, SDL_FALSE, &m_DeviceSpec,
                                   nullptr, SDL_AUDIO_ALLOW_ANY_CHANGE);

    if (m_Device == 0) {
        std::cerr << "failed open audio device: " << SDL_GetError();
        throw std::runtime_error("audio failed");
    }

    SDL_PauseAudioDevice(m_Device, SDL_FALSE);
}

void SDLSoundMixer::add(const std::string &name, const std::string &path) {
    auto sound = std::make_shared<SDLSoundBuffer>(path, m_Device, m_DeviceSpec);
    SDL_LockAudioDevice(m_Device);
    m_Sounds.push_back(sound);
    m_SoundsMap[name] = sound;
    SDL_UnlockAudioDevice(m_Device);
}

void SDLSoundMixer::play(const std::string &name, float volume,
                         SoundBuffer::Properties properties) {
    auto &sound = m_SoundsMap[name];
    sound->play(properties, volume);
}

void SDLSoundMixer::stop() {
    for (auto &sound : m_Sounds) {
        sound->stop();
    }
}

void SDLSoundMixer::stop(const std::string &name) {
    auto &sound = m_SoundsMap[name];
    sound->stop();
}

void SDLSoundMixer::audioCallback(void *userData, uint8_t *stream,
                                  int streamSize) {
    std::fill_n(stream, streamSize, '\0');
    auto mixer = static_cast<SDLSoundMixer *>(userData);

    for (auto &sound : mixer->m_Sounds) {
        if (!sound->isPlaying()) {
            continue;
        }

        uint8_t *current = &sound->data()[sound->position()];
        int volume = static_cast<int>(sound->volume() * SDL_MIX_MAXVOLUME);

        if (sound->rest() <= static_cast<uint32_t>(streamSize)) {
            // copy rest to buffer
            SDL_MixAudioFormat(stream, current, mixer->m_DeviceSpec.format,
                               sound->rest(), volume);
            sound->move(sound->rest());
        } else {
            SDL_MixAudioFormat(stream, current, mixer->m_DeviceSpec.format,
                               static_cast<uint32_t>(streamSize), volume);
            sound->move(static_cast<uint32_t>(streamSize));
        }

        if (sound->isEnd()) {
            if (sound->isLooped()) {
                sound->reset();
            } else {
                sound->stop();
            }
        }
    }
}

void SDLSoundMixer::clear() {
    stop();
    m_Sounds.clear();
    m_SoundsMap.clear();
}

} // namespace Engine