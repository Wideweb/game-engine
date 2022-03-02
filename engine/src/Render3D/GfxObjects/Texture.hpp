#pragma once

#include "GfxImage.hpp"

#include <string>

namespace Engine {

class Texture : public GfxImage {
  public:
    enum class TextureType { COLOR, CUBE_MAP, DEPTH_BUFFER };

    TextureType type;
    DataFormat dataFormat;
    DataType dataType;

    void bind() const override;
    void unbind() const override;
    void free() override;

    void resize(unsigned int width, unsigned int height) override;

    static Texture createDepthBuffer(int width, int height);
    static Texture createCubeDepthBuffer(int width, int height);

    static Texture createRGBA8FBuffer(int width, int height);
    static Texture createRGBA16FBuffer(int width, int height);
    static Texture createRGBA32FBuffer(int width, int height);

    static Texture createRGB8FBuffer(int width, int height);
    static Texture createRGB16FBuffer(int width, int height);
    static Texture createRGB32FBuffer(int width, int height);

    static Texture createRGB8IBuffer(int width, int height);
    static Texture createR32IBuffer(int width, int height);
};

} // namespace Engine
