#pragma once

#include <iostream>
#include <cassert>

#define GL_CHECK()                                                             \
    {                                                                          \
        const int err = static_cast<int>(glGetError());                        \
        if (err != GL_NO_ERROR) {                                              \
            switch (err) {                                                     \
            case GL_INVALID_ENUM:                                              \
                std::cerr << "GL_INVALID_ENUM" << std::endl;                   \
                break;                                                         \
            case GL_INVALID_VALUE:                                             \
                std::cerr << "GL_INVALID_VALUE" << std::endl;                  \
                break;                                                         \
            case GL_INVALID_OPERATION:                                         \
                std::cerr << "GL_INVALID_OPERATION" << std::endl;              \
                break;                                                         \
            case GL_INVALID_FRAMEBUFFER_OPERATION:                             \
                std::cerr << "GL_INVALID_FRAMEBUFFER_OPERATION" << std::endl;  \
                break;                                                         \
            case GL_OUT_OF_MEMORY:                                             \
                std::cerr << "GL_OUT_OF_MEMORY" << std::endl;                  \
                break;                                                         \
            }                                                                  \
            assert(false);                                                     \
        }                                                                      \
    }
