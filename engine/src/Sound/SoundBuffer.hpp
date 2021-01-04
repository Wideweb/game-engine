#pragma once

#include <stdint.h>
#include <string>

namespace Engine {

class SoundBuffer {
  public:
    enum class Properties { Once, Looped };

    virtual ~SoundBuffer() = default;

    virtual void *getDevice() = 0;

    virtual void play(const Properties, float volume) = 0;
    virtual void reset() = 0;
    virtual void stop() = 0;
    virtual bool isPlaying() = 0;
    virtual bool isLooped() = 0;
    virtual bool isEnd() = 0;
    virtual uint32_t rest() = 0;
    virtual void move(int) = 0;
    virtual uint32_t position() = 0;
    virtual uint8_t *data() = 0;
    virtual float volume() = 0;

    static SoundBuffer *create(const std::string &path);
};

} // namespace Engine