#pragma once

#include "GfxImage.hpp"

#include <string>

namespace Engine {

class Texture : public GfxImage {
  public:
    enum class TextureType { NONE, COLOR, CUBE_MAP, DEPTH_BUFFER };

    TextureType type;
    DataFormat dataFormat;
    DataType dataType;

    void bind() const override;
    void unbind() const override;
    void free() override;

    void resize(unsigned int width, unsigned int height) override;

    static Texture getEmpty();

    static Texture createDepthBuffer(int width, int height);

    static Texture createRGBA8FBuffer(int width, int height);
    static Texture createRGBA16FBuffer(int width, int height);
    static Texture createRGBA32FBuffer(int width, int height);

    static Texture createRGB8FBuffer(int width, int height);
    static Texture createRGB16FBuffer(int width, int height, void *data = nullptr);
    static Texture createRGB32FBuffer(int width, int height);

    static Texture createRGB8IBuffer(int width, int height);
    static Texture createRGBA8Buffer(int width, int height, void *data = nullptr);

    static Texture createR32IBuffer(int width, int height);
    static Texture createR8Buffer(int width, int height);
    static Texture createR16FBuffer(int width, int height);
    static Texture createR32FBuffer(int width, int height);

    static Texture createTrueTypeGlyph(int width, int height, unsigned char *data);
};

class CubeMapTexture : public Texture {
  public:
    Texture faces[6];

    static CubeMapTexture createCubeDepthBuffer(int width, int height);
    static CubeMapTexture createCubeMap();
};

} // namespace Engine
