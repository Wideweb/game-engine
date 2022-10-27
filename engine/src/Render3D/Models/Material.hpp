#pragma once

#include <vector>
#include <string>

#include "Texture.hpp"
#include "FlatDictionary.hpp"
#include "Shader.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace Engine {

class  Material {
  private:
    Shader* m_Shader;
    FlatDictionary<std::string, Shader::Property> m_Properties;
    bool m_DepthTest = true;

  public:
    Material();
    Material(Shader* shader);

    void bind();
    void apply();

    void setDepthTest(bool depthTest) { m_DepthTest = depthTest; }
    bool getDepthTest() { return m_DepthTest; }

    void setShader(Shader* shader);
    bool setInt(const std::string& name, int32_t value);
    bool setFloat(const std::string& name, float value);
    bool setFloat2(const std::string& name, glm::vec2 value);
    bool setFloat3(const std::string& name, glm::vec3 value);
    bool setFloat4(const std::string& name, glm::vec4 value);
    bool setMatrix4(const std::string& name, glm::mat4 value);
    bool setTexture(const std::string& name, const Texture* value);
    bool setPropertyValue(const std::string& name, Shader::Property property);

    bool addInt(const std::string& name, int32_t value = 0);
    bool addFloat(const std::string& name, float value = 0.0f);
    bool addFloat2(const std::string& name, glm::vec2 value= glm::vec2(0.0f));
    bool addFloat3(const std::string& name, glm::vec3 value = glm::vec3(0.0f));
    bool addFloat4(const std::string& name, glm::vec4 value= glm::vec4(0.0f));
    bool addMatrix4(const std::string& name, glm::mat4 value = glm::mat4(0.0f));
    bool addTexture(const std::string& name, const Texture* value = nullptr);
    bool addProperty(const std::string& name, Shader::Property property);

    const Shader::Property& getProperty(const std::string& name) const;

    const std::vector<std::string>& keys() const;
    const std::vector<Shader::Property>& properties() const;
    bool empty() const;

    // Texture diffuseMap;
    // Texture specularMap;
    // Texture normalMap;
    // float specular = 1.0f;
    // float shininess = 128.0f;

    // PBR
    // Texture metallicMap;
    // Texture roughnessMap;
    // Texture ambientOcclusionMap;
};

} // namespace Engine
