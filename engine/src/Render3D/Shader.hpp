#pragma once

#include <glm/glm.hpp>

#include "glad/glad.h"
#include <string>
#include <unordered_map>

namespace Engine {

class Shader {
  private:
    GLuint m_Program;
    std::unordered_map<std::string, GLint> m_UniformLocationMap;

  public:
    Shader(const std::string &vertexSrc, const std::string &fragmentSrc);
    ~Shader();

    void bind() const;
    void unbind() const;

    void setInt(const std::string &name, int value);
    void setFloat(const std::string &name, float value);
    void setFloat2(const std::string &name, float value1, float value2);
    void setFloat3(const std::string &name, float value, float value2,
                   float value3);
    void setFloat3(const std::string &name, glm::vec3 value);
    void setMatrix4(const std::string &name, const float *matrix);
    void setMatrix2x3(const std::string &name,
                      const std::vector<float> &matrix);
    void setMatrix2(const std::string &name, const std::vector<float> &matrix);

  private:
    void compile(const std::string &vertexSrc, const std::string &fragmentSrc);
    GLuint compileShader(GLenum type, const std::string &source);
    GLuint getUniformLocation(const std::string &name);
};

} // namespace Engine
