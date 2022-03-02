#pragma once

#include "GfxObject.hpp"

namespace Engine {

class GfxImage : public GfxObject {
  public:
    enum class InternalFormat {
        NONE,
        DEPTH_COMPONENT,
        DEPTH_STENCIL,

        R8F,
        R8I,
        R16I,
        R32I,

        RGB8F,
        RGB8I,
        RGB16F,
        RGB32F,

        RGBA8F,
        RGBA16F,
        RGBA32F
    };

    enum class DataFormat { DEPTH_COMPONENT, DEPTH_STENCIL, RED, RED_INTEGER, RG, RGB, RGBA };
    enum class DataType { UNSIGNED_BYTE, INT, FLOAT };

    unsigned int width = 0, height = 0;

    InternalFormat format = InternalFormat::NONE;

  public:
    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    virtual void free() = 0;

    virtual void resize(unsigned int width, unsigned int height) = 0;

    static unsigned int getNativeDataType(DataType dataType);
    static unsigned int getNativeDataFormat(DataFormat dataFormat);
    static unsigned int getNativeFormat(InternalFormat format);
    static DataType formatToDataType(InternalFormat format);
    static DataFormat formatToDataFormat(InternalFormat format);
};

} // namespace Engine