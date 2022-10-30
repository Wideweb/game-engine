#pragma once

#include "GfxObject.hpp"
#include "Texture.hpp"
#include "FlatDictionary.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <string>
#include <unordered_map>
#include <vector>

namespace Engine {

using ShaderId = GfxObjectId;

constexpr ShaderId c_NoShader = 0;

class Shader : public GfxObject {
  public:
    struct Property {
      enum class Type {
        UNKNOWN,
        INT1,
        FLOAT1,
        FLOAT2,
        FLOAT3,
        FLOAT4,
        MATRIX4,
        TEXTURE
      };

      union Value {
        int32_t int1;
        float float1;
        glm::vec2 float2;
        glm::vec3 float3;
        glm::vec4 float4;
        glm::mat4 matrix4;
        const Texture* texture;
      };

      Type type;
      Value value;
      bool empty;

      Property(): type(Type::UNKNOWN), empty(true) {}
      Property(Type type): type(type), empty(true) {
        value.texture = nullptr;
      }
   };

  private:
    std::unordered_map<std::string, int> m_UniformLocationMap;
    std::unordered_map<std::string, int> m_TextureIndex;
    FlatDictionary<std::string, Shader::Property::Type> m_Uniforms;

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

    void set(const std::string &name, const Property property);
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
    void setTexture(const std::string &name, const Texture *texture);

    const std::vector<std::string>& uniformKeys() const { return m_Uniforms.keys(); };
    const std::vector<Shader::Property::Type>& uniformTypes() const { return m_Uniforms.values(); };

  private:
    void compile(const std::string &vertexSrc, const std::string &fragmentSrc,
                 std::vector<std::string> transformFeedbackVaryings);
    void compile(const std::string &vertexSrc, const std::string &fragmentSrc, const std::string &geometrySrc);
    unsigned int compileShader(unsigned int type, const std::string &source);
    void readUniforms();
    int getUniformLocation(const std::string &name);

    static Shader::Property::Type fromNativeType(int type);
};

} // namespace Engine
