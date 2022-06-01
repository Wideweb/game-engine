#pragma once

#include "GfxObject.hpp"
#include "Texture.hpp"

#include <glm/glm.hpp>

#include <string>
#include <unordered_map>
#include <vector>

namespace Engine {

using ShaderId = GfxObjectId;

constexpr ShaderId c_NoShader = 0;

class Shader : public GfxObject {
  private:
    std::unordered_map<std::string, int> m_UniformLocationMap;
    std::unordered_map<std::string, int> m_TextureIndex;

  public:
    Shader();
    Shader(const std::string &computeSrc);
    Shader(const std::string &vertexSrc, const std::string &fragmentSrc);
    Shader(const std::string &vertexSrc, const std::string &fragmentSrc,
           std::vector<std::string> transformFeedbackVaryings);
    Shader(const std::string &vertexSrc, const std::string &fragmentSrc, const std::string &geometrySrc);

    Shader(const Shader &shader) = delete;
    Shader &operator=(const Shader &shader) = delete;

    Shader(Shader &&shader) = default;
    Shader &operator=(Shader &&shader) = default;

    void bind() const override;
    void unbind() const override;
    void free() override;

    void setInt(const std::string &name, int value);
    void setFloat(const std::string &name, float value);
    void setFloat2(const std::string &name, float value1, float value2);
    void setFloat2(const std::string &name, glm::vec2 value);
    void setFloat3(const std::string &name, float value, float value2, float value3);
    void setFloat3(const std::string &name, glm::vec3 value);
    void setFloat4(const std::string &name, glm::vec4 value);
    void setMatrix4(const std::string &name, const glm::mat4 &matrix);
    void setMatrix2x3(const std::string &name, const std::vector<float> &matrix);
    void setMatrix2(const std::string &name, const std::vector<float> &matrix);
    void setTexture(const std::string &name, const Texture &texture);

  private:
    void compile(const std::string &vertexSrc, const std::string &fragmentSrc,
                 std::vector<std::string> transformFeedbackVaryings);
    void compile(const std::string &vertexSrc, const std::string &fragmentSrc, const std::string &geometrySrc);
    unsigned int compileShader(unsigned int type, const std::string &source);
    int getUniformLocation(const std::string &name);
};

} // namespace Engine
