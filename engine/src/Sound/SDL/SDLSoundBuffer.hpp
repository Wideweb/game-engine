#pragma once

#include "SoundBuffer.hpp"

#include <SDL.h>
#include <string>

namespace Engine {

class SDLSoundBuffer : public SoundBuffer {
  private:
    uint8_t *m_Buffer;
    float m_Volume;
    uint32_t m_Length = 0;
    uint32_t m_Index = 0;
    SDL_AudioDeviceID m_Device;
    bool m_Playing = false;
    bool m_Looped = false;

  public:
    SDLSoundBuffer(const std::string &path, SDL_AudioDeviceID device,
                   SDL_AudioSpec deviceSpec);
    virtual ~SDLSoundBuffer();

    virtual void play(const Properties, float volume) override;
    virtual void *getDevice() override;

    virtual bool isPlaying() override { return m_Playing; }
    virtual bool isLooped() override { return m_Looped; }
    virtual bool isEnd() override { return m_Length == m_Index; }
    virtual void reset() override { m_Index = 0; }
    virtual void stop() override { m_Playing = false; }
    virtual uint32_t rest() override { return m_Length - m_Index; }
    virtual void move(int step) override { m_Index += step; }
    virtual uint32_t position() override { return m_Index; }
    virtual uint8_t *data() override { return m_Buffer; }
    virtual float volume() override { return m_Volume; }
};

} // namespace Engine
