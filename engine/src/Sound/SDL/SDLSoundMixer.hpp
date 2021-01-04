#pragma once

#include "SDLSoundBuffer.hpp"
#include "SoundMixer.hpp"

#include <SDL.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace Engine {

class SDLSoundMixer : public SoundMixer {
  private:
    SDL_AudioDeviceID m_Device;
    SDL_AudioSpec m_DeviceSpec;

    std::vector<std::shared_ptr<SDLSoundBuffer>> m_Sounds;
    std::unordered_map<std::string, std::shared_ptr<SDLSoundBuffer>>
        m_SoundsMap;

  public:
    SDLSoundMixer();

    virtual void init() override;

    virtual void add(const std::string &name, const std::string &path) override;
    virtual void play(const std::string &name, float volume,
                      SoundBuffer::Properties properties) override;
    virtual void stop(const std::string &name) override;
    virtual void stop() override;
    virtual void clear() override;

    static void audioCallback(void *, uint8_t *, int);
};

} // namespace Engine
