#include "Shader.hpp"

#include "glad/glad.h"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

namespace Engine {

Shader::Shader() {}

Shader::Shader(const std::string &computeSrc) {
    GLuint computeShader = compileShader(GL_COMPUTE_SHADER, computeSrc);
    if (computeShader == 0) {
        return;
    }

    id = glCreateProgram();
    if (id == 0) {
        std::cerr << "failed to create gl program";
        return;
    }

    glAttachShader(id, computeShader);

    glLinkProgram(id);

    GLint linked_status = 0;
    glGetProgramiv(id, GL_LINK_STATUS, &linked_status);
    if (linked_status == 0) {
        GLint infoLen = 0;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLen);

        std::vector<char> infoLog(static_cast<size_t>(infoLen));
        glGetProgramInfoLog(id, infoLen, nullptr, infoLog.data());

        std::cerr << "Error linking program:\n" << infoLog.data();
        glDeleteProgram(id);
        return;
    }

    glDetachShader(id, computeShader);
    glDeleteShader(computeShader);
}

Shader::Shader(const std::string &vertexSrc, const std::string &fragmentSrc,
               std::vector<std::string> transformFeedbackVaryings) {
    compile(vertexSrc, fragmentSrc, transformFeedbackVaryings);
}

Shader::Shader(const std::string &vertexSrc, const std::string &fragmentSrc) {
    compile(vertexSrc, fragmentSrc, std::vector<std::string>{});
}

Shader::Shader(const std::string &vertexSrc, const std::string &fragmentSrc, const std::string &geometrySrc) {
    compile(vertexSrc, fragmentSrc, geometrySrc);
}

void Shader::bind() const { glUseProgram(id); }

void Shader::unbind() const { glUseProgram(0); }

void Shader::free() {
    if (!empty()) {
        glDeleteProgram(id);
        setEmpty();
    }
}

void Shader::compile(const std::string &vertexSrc, const std::string &fragmentSrc,
                     std::vector<std::string> transformFeedbackVaryings) {

    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSrc);
    if (vertexShader == 0) {
        return;
    }

    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSrc);
    if (fragmentShader == 0) {
        return;
    }

    id = glCreateProgram();
    if (id == 0) {
        std::cerr << "failed to create gl program";
        return;
    }

    if (!transformFeedbackVaryings.empty()) {
        std::vector<char *> names;
        std::transform(transformFeedbackVaryings.begin(), transformFeedbackVaryings.end(), std::back_inserter(names),
                       [](std::string &name) { return name.data(); });

        glTransformFeedbackVaryings(id, static_cast<GLsizei>(names.size()), names.data(), GL_INTERLEAVED_ATTRIBS);
    }

    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);

    glLinkProgram(id);

    GLint linked_status = 0;
    glGetProgramiv(id, GL_LINK_STATUS, &linked_status);
    if (linked_status == 0) {
        GLint infoLen = 0;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLen);

        std::vector<char> infoLog(static_cast<size_t>(infoLen));
        glGetProgramInfoLog(id, infoLen, nullptr, infoLog.data());

        std::cerr << "Error linking program:\n" << infoLog.data();
        glDeleteProgram(id);
        return;
    }

    glDetachShader(id, vertexShader);
    glDetachShader(id, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::compile(const std::string &vertexSrc, const std::string &fragmentSrc, const std::string &geometrySrc) {

    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSrc);
    if (vertexShader == 0) {
        return;
    }

    GLuint geometryShader = compileShader(GL_GEOMETRY_SHADER, geometrySrc);
    if (geometryShader == 0) {
        return;
    }

    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSrc);
    if (fragmentShader == 0) {
        return;
    }

    id = glCreateProgram();
    if (id == 0) {
        std::cerr << "failed to create gl program";
        return;
    }

    glAttachShader(id, vertexShader);
    glAttachShader(id, geometryShader);
    glAttachShader(id, fragmentShader);

    glLinkProgram(id);

    GLint linked_status = 0;
    glGetProgramiv(id, GL_LINK_STATUS, &linked_status);
    if (linked_status == 0) {
        GLint infoLen = 0;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLen);

        std::vector<char> infoLog(static_cast<size_t>(infoLen));
        glGetProgramInfoLog(id, infoLen, nullptr, infoLog.data());

        std::cerr << "Error linking program:\n" << infoLog.data();
        glDeleteProgram(id);
        return;
    }

    glDetachShader(id, vertexShader);
    glDetachShader(id, geometryShader);
    glDetachShader(id, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(geometryShader);
    glDeleteShader(fragmentShader);
}

GLuint Shader::compileShader(unsigned int type, const std::string &source) {
    GLuint shader = glCreateShader(type);

    const char *sourceCStr = source.c_str();
    glShaderSource(shader, 1, &sourceCStr, nullptr);
    glCompileShader(shader);

    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

        std::vector<char> infoLog(static_cast<size_t>(infoLen));
        glGetShaderInfoLog(shader, infoLen, nullptr, infoLog.data());

        glDeleteShader(shader);

        std::cerr << "Error compiling shader(vertex)\n" << infoLog.data();
        return 0;
    }

    return shader;
}

void Shader::setInt(const std::string &name, int value) {
    GLint location = getUniformLocation(name);
    if (location == -1)
        return;
    glUniform1i(location, value);
}

void Shader::setFloat(const std::string &name, float value) {
    GLint location = getUniformLocation(name);
    if (location == -1)
        return;
    glUniform1f(location, value);
}

void Shader::setFloat2(const std::string &name, float value1, float value2) {
    GLint location = getUniformLocation(name);
    if (location == -1)
        return;
    glUniform2f(location, value1, value2);
}

void Shader::setFloat2(const std::string &name, glm::vec2 value) {
    GLint location = getUniformLocation(name);
    if (location == -1)
        return;
    glUniform2f(location, value.x, value.y);
}

void Shader::setFloat3(const std::string &name, float value1, float value2, float value3) {
    GLint location = getUniformLocation(name);
    if (location == -1)
        return;
    glUniform3f(location, value1, value2, value3);
}

void Shader::setFloat3(const std::string &name, glm::vec3 value) {
    GLint location = getUniformLocation(name);
    if (location == -1)
        return;
    glUniform3f(location, value.x, value.y, value.z);
}

void Shader::setFloat4(const std::string &name, glm::vec4 value) {
    GLint location = getUniformLocation(name);
    if (location == -1)
        return;
    glUniform4f(location, value.x, value.y, value.z, value.w);
}

void Shader::setMatrix4(const std::string &name, const glm::mat4 &matrix) {
    GLint location = getUniformLocation(name);
    if (location == -1)
        return;
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setMatrix2x3(const std::string &name, const std::vector<float> &matrix) {
    GLint location = getUniformLocation(name);
    if (location == -1)
        return;
    glUniformMatrix2x3fv(location, 1, GL_FALSE, matrix.data());
}

void Shader::setMatrix2(const std::string &name, const std::vector<float> &matrix) {
    GLint location = getUniformLocation(name);
    if (location == -1)
        return;
    glUniformMatrix2fv(location, 1, GL_FALSE, matrix.data());
}

void Shader::setTexture(const std::string &name, const Texture &texture) {
    GLint location = glGetUniformLocation(id, name.c_str());
    if (location == -1) {
        return;
    }

    unsigned int index = 0;
    auto it = m_TextureIndex.find(name);
    if (it != m_TextureIndex.end()) {
        index = it->second;
    } else {
        index = m_TextureIndex.size();
        m_TextureIndex.emplace(name, index);
    }
    glActiveTexture(GL_TEXTURE0 + index);
    texture.bind();
    glUniform1i(location, index);
}

int Shader::getUniformLocation(const std::string &name) {
    auto it = m_UniformLocationMap.find(name);
    if (it != m_UniformLocationMap.end()) {
        return it->second;
    }
    GLint location = glGetUniformLocation(id, name.c_str());
    if (location != -1) {
        m_UniformLocationMap[name] = location;
    }
    return location;
}

} // namespace Engine
