#include <iostream>
#include <vector>

#include "Shader.hpp"

namespace Engine {

Shader::Shader(const std::string &vertexSrc, const std::string &fragmentSrc) {
    compile(vertexSrc, fragmentSrc);
}

Shader::Shader(const std::string &vertexSrc, const std::string &fragmentSrc,
               const std::string &geometrySrc) {
    compile(vertexSrc, fragmentSrc, geometrySrc);
}

Shader::~Shader() { glDeleteProgram(m_Program); }

void Shader::bind() const {
    glUseProgram(m_Program);
    // std::cout << glGetError() << std::endl;
}

void Shader::unbind() const { glUseProgram(0); }

void Shader::compile(const std::string &vertexSrc,
                     const std::string &fragmentSrc) {

    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSrc);
    if (vertexShader == 0) {
        return;
    }

    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSrc);
    if (fragmentShader == 0) {
        return;
    }

    GLuint programId = glCreateProgram();
    if (programId == 0) {
        std::cerr << "failed to create gl program";
        return;
    }

    glAttachShader(programId, vertexShader);
    glAttachShader(programId, fragmentShader);

    glLinkProgram(programId);

    GLint linked_status = 0;
    glGetProgramiv(programId, GL_LINK_STATUS, &linked_status);
    if (linked_status == 0) {
        GLint infoLen = 0;
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLen);

        std::vector<char> infoLog(static_cast<size_t>(infoLen));
        glGetProgramInfoLog(programId, infoLen, nullptr, infoLog.data());

        std::cerr << "Error linking program:\n" << infoLog.data();
        glDeleteProgram(programId);
        return;
    }

    glDetachShader(programId, vertexShader);
    glDetachShader(programId, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    m_Program = programId;
}

void Shader::compile(const std::string &vertexSrc,
                     const std::string &fragmentSrc,
                     const std::string &geometrySrc) {

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

    GLuint programId = glCreateProgram();
    if (programId == 0) {
        std::cerr << "failed to create gl program";
        return;
    }

    glAttachShader(programId, vertexShader);
    glAttachShader(programId, geometryShader);
    glAttachShader(programId, fragmentShader);

    glLinkProgram(programId);

    GLint linked_status = 0;
    glGetProgramiv(programId, GL_LINK_STATUS, &linked_status);
    if (linked_status == 0) {
        GLint infoLen = 0;
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLen);

        std::vector<char> infoLog(static_cast<size_t>(infoLen));
        glGetProgramInfoLog(programId, infoLen, nullptr, infoLog.data());

        std::cerr << "Error linking program:\n" << infoLog.data();
        glDeleteProgram(programId);
        return;
    }

    glDetachShader(programId, vertexShader);
    glDetachShader(programId, geometryShader);
    glDetachShader(programId, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(geometryShader);
    glDeleteShader(fragmentShader);

    m_Program = programId;
}

GLuint Shader::compileShader(GLenum type, const std::string &source) {
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
    glUniform1i(location, value);
}

void Shader::setFloat(const std::string &name, float value) {
    GLint location = getUniformLocation(name);
    glUniform1f(location, value);
    // std::cout << glGetError() << std::endl;
}

void Shader::setFloat2(const std::string &name, float value1, float value2) {
    GLint location = getUniformLocation(name);
    glUniform2f(location, value1, value2);
    // std::cout << glGetError() << std::endl;
}

void Shader::setFloat3(const std::string &name, float value1, float value2,
                       float value3) {
    GLint location = getUniformLocation(name);
    glUniform3f(location, value1, value2, value3);
    // std::cout << glGetError() << std::endl;
}

void Shader::setFloat3(const std::string &name, glm::vec3 value) {
    GLint location = getUniformLocation(name);
    glUniform3f(location, value.x, value.y, value.z);
    // std::cout << glGetError() << std::endl;
}

void Shader::setMatrix4(const std::string &name, const float *matrix) {
    GLint location = getUniformLocation(name);
    glUniformMatrix4fv(location, 1, GL_FALSE, matrix);
    // std::cout << glGetError() << std::endl;
}

void Shader::setMatrix2x3(const std::string &name,
                          const std::vector<float> &matrix) {
    GLint location = getUniformLocation(name);
    glUniformMatrix2x3fv(location, 1, GL_FALSE, matrix.data());
}

void Shader::setMatrix2(const std::string &name,
                        const std::vector<float> &matrix) {
    GLint location = getUniformLocation(name);
    glUniformMatrix2fv(location, 1, GL_FALSE, matrix.data());
}

GLint Shader::getUniformLocation(const std::string &name) {
    auto it = m_UniformLocationMap.find(name);
    if (it != m_UniformLocationMap.end()) {
        return it->second;
    }
    GLint location = glGetUniformLocation(m_Program, name.c_str());
    // std::cout << glGetError() << std::endl;
    m_UniformLocationMap[name] = location;
    return location;
}

} // namespace Engine
