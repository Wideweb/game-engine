#include "GfxImage.hpp"

#include "glad/glad.h"

#include <stdexcept>

namespace Engine {

unsigned int GfxImage::getNativeDataFormat(DataFormat dataFormat) {
    switch (dataFormat) {
    case DataFormat::DEPTH_COMPONENT:
        return GL_DEPTH_COMPONENT;
    case DataFormat::DEPTH_STENCIL:
        return GL_DEPTH_STENCIL;
    case DataFormat::RED:
        return GL_RED;
    case DataFormat::RED_INTEGER:
        return GL_RED_INTEGER;
    case DataFormat::RG:
        return GL_RG;
    case DataFormat::RGB:
        return GL_RGB;
    case DataFormat::RGBA:
        return GL_RGBA;
    default:
        throw std::invalid_argument("GfxImage::getNativeDataFormat: Invalid Data Format.");
    }
}

unsigned int GfxImage::getNativeDataType(DataType dataType) {
    switch (dataType) {
    case DataType::UNSIGNED_BYTE:
        return GL_UNSIGNED_BYTE;
    case DataType::INT:
        return GL_INT;
    case DataType::FLOAT:
        return GL_FLOAT;
    default:
        throw std::invalid_argument("GfxImage::getNativeDataType: Invalid Data Type.");
    }
}

unsigned int GfxImage::getNativeFormat(InternalFormat format) {
    switch (format) {
    case InternalFormat::DEPTH_COMPONENT:
        return GL_DEPTH_COMPONENT;
    case InternalFormat::DEPTH_STENCIL:
        return GL_DEPTH_STENCIL;

    case InternalFormat::R8F:
        return GL_R8;
    case InternalFormat::R8I:
        return GL_R8I;
    case InternalFormat::R16I:
        return GL_R16I;
    case InternalFormat::R16F:
        return GL_R16F;
    case InternalFormat::R32F:
        return GL_R32F;
    case InternalFormat::R32I:
        return GL_R32I;

    case InternalFormat::RGB8F:
        return GL_RGB8;
    case InternalFormat::RGB8I:
        return GL_RGB8I;
    case InternalFormat::RGB16F:
        return GL_RGB16F;
    case InternalFormat::RGB32F:
        return GL_RGB32F;

    case InternalFormat::RGBA8F:
        return GL_RGBA8;
    case InternalFormat::RGBA16F:
        return GL_RGBA16F;
    case InternalFormat::RGBA32F:
        return GL_RGBA32F;
    default:
        throw std::invalid_argument("GfxImage::getNativeFormat: invalid internal format.");
    }
}

GfxImage::DataType GfxImage::formatToDataType(InternalFormat format) {
    switch (format) {
    case InternalFormat::DEPTH_COMPONENT:
        return DataType::FLOAT;
    case InternalFormat::DEPTH_STENCIL:
        return DataType::FLOAT;

    case InternalFormat::R8F:
        return DataType::FLOAT;
    case InternalFormat::R8I:
        return DataType::INT;
    case InternalFormat::R16I:
        return DataType::INT;
    case InternalFormat::R16F:
        return DataType::FLOAT;
    case InternalFormat::R32F:
        return DataType::FLOAT;
    case InternalFormat::R32I:
        return DataType::INT;

    case InternalFormat::RGB8F:
        return DataType::FLOAT;
    case InternalFormat::RGB8I:
        return DataType::INT;
    case InternalFormat::RGB16F:
        return DataType::FLOAT;

    case InternalFormat::RGBA8F:
        return DataType::FLOAT;
    case InternalFormat::RGBA16F:
        return DataType::FLOAT;
    case InternalFormat::RGBA32F:
        return DataType::FLOAT;
    default:
        throw std::invalid_argument("GfxImage: invalid internal format.");
    }
}

GfxImage::DataFormat GfxImage::formatToDataFormat(InternalFormat format) {
    switch (format) {
    case InternalFormat::DEPTH_COMPONENT:
        return DataFormat::DEPTH_COMPONENT;
    case InternalFormat::DEPTH_STENCIL:
        return DataFormat::DEPTH_STENCIL;

    case InternalFormat::R8F:
        return DataFormat::RED;
    case InternalFormat::R8I:
        return DataFormat::RED_INTEGER;
    case InternalFormat::R16I:
        return DataFormat::RED_INTEGER;
    case InternalFormat::R16F:
        return DataFormat::RED;
    case InternalFormat::R32F:
        return DataFormat::RED;
    case InternalFormat::R32I:
        return DataFormat::RED_INTEGER;

    case InternalFormat::RGB8F:
        return DataFormat::RGB;
    case InternalFormat::RGB8I:
        return DataFormat::RGB;
    case InternalFormat::RGB16F:
        return DataFormat::RGB;

    case InternalFormat::RGBA8F:
        return DataFormat::RGBA;
    case InternalFormat::RGBA16F:
        return DataFormat::RGBA;
    case InternalFormat::RGBA32F:
        return DataFormat::RGBA;
    default:
        throw std::invalid_argument("GfxImage::formatToDataFormat: invalid internal format.");
    }
}

} // namespace Engine