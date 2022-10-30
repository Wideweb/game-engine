#include "Material.hpp"

namespace Engine {

Material::Material(): m_Shader(nullptr) {}

Material::Material(Shader* shader): m_Shader(shader) {
    size_t uniformsNumber = shader->uniformKeys().size();
    for (size_t i = 0; i < uniformsNumber; i++) {
        addProperty(shader->uniformKeys()[i], {shader->uniformTypes()[i]});
    }
}

void Material::bind() {
    if (m_Shader != nullptr) {
        m_Shader->bind();
    }
}

void Material::apply(Shader* shader) {
    Shader* usedShader = shader == nullptr ? m_Shader : shader;
    for (size_t i = 0; i < m_Properties.size(); i++) {
        auto& property = m_Properties.values()[i];
        if (property.empty) {
            continue;
        }

        usedShader->set(m_Properties.keys()[i], property);
    }
}

void Material::setShader(Shader* shader) {
    m_Shader = shader;
}

bool Material::setInt(const std::string& name, int32_t value) {
    Shader::Property property;
    property.type = Shader::Property::Type::INT1;
    property.value.int1 = value;
    return setPropertyValue(name, property);
}

bool Material::setFloat(const std::string& name, float value) {
    Shader::Property property;
    property.type = Shader::Property::Type::FLOAT1;
    property.value.float1 = value;
    return setPropertyValue(name, property);
}

bool Material::setFloat2(const std::string& name, glm::vec2 value) {
    Shader::Property property;
    property.type = Shader::Property::Type::FLOAT2;
    property.value.float2 = value;
    return setPropertyValue(name, property);
}

bool Material::setFloat3(const std::string& name, glm::vec3 value) {
    Shader::Property property;
    property.type = Shader::Property::Type::FLOAT3;
    property.value.float3 = value;
    return setPropertyValue(name, property);
}

bool Material::setFloat4(const std::string& name, glm::vec4 value) {
    Shader::Property property;
    property.type = Shader::Property::Type::FLOAT4;
    property.value.float4 = value;
    return setPropertyValue(name, property);
}

bool Material::setMatrix4(const std::string& name, glm::mat4 value) {
    Shader::Property property;
    property.type = Shader::Property::Type::MATRIX4;
    property.value.matrix4 = value;
    return setPropertyValue(name, property);
}

bool Material::setTexture(const std::string& name, const Texture* value) {
    Shader::Property property;
    property.type = Shader::Property::Type::TEXTURE;
    property.value.texture = value;
    return setPropertyValue(name, property);
}

bool Material::setPropertyValue(const std::string& name, Shader::Property property) {
    if (!m_Properties.hasKey(name)) {
        if (m_Shader == nullptr) {
            addProperty(name, property);
        } else {
            std::cerr << "Material property type is unknown: " << name << "\n";
            return false;
        }
    }
    auto& currentProperty = m_Properties[name];
    currentProperty.empty = false;
    if (currentProperty.type != property.type) {
        return true;
    }

    currentProperty.value = property.value;
    return true;
}

bool Material::addInt(const std::string& name, int32_t value) {
    Shader::Property property;
    property.type = Shader::Property::Type::INT1;
    property.value.int1 = value;
    return addProperty(name, property);
}

bool Material::addFloat(const std::string& name, float value) {
    Shader::Property property;
    property.type = Shader::Property::Type::FLOAT1;
    property.value.float1 = value;
    return addProperty(name, property);
}

bool Material::addFloat2(const std::string& name, glm::vec2 value) {
    Shader::Property property;
    property.type = Shader::Property::Type::FLOAT2;
    property.value.float2 = value;
    return addProperty(name, property);
}

bool Material::addFloat3(const std::string& name, glm::vec3 value) {
    Shader::Property property;
    property.type = Shader::Property::Type::FLOAT3;
    property.value.float3 = value;
    return addProperty(name, property);
}

bool Material::addFloat4(const std::string& name, glm::vec4 value) {
    Shader::Property property;
    property.type = Shader::Property::Type::FLOAT4;
    property.value.float4 = value;
    return addProperty(name, property);
}

bool Material::addMatrix4(const std::string& name, glm::mat4 value) {
    Shader::Property property;
    property.type = Shader::Property::Type::MATRIX4;
    property.value.matrix4 = value;
    return addProperty(name, property);
}

bool Material::addTexture(const std::string& name, const Texture* value) {
    Shader::Property property;
    property.type = Shader::Property::Type::TEXTURE;
    property.value.texture = value;
    return addProperty(name, property);
}

bool Material::addProperty(const std::string& name, Shader::Property property) {
    if (m_Properties.hasKey(name)) {
        return false;
    }
    m_Properties.add(name, property);
    return true;
}

const Shader::Property& Material::getProperty(const std::string& name) const {
    return m_Properties.getValue(name);
}

const std::vector<std::string>& Material::keys() const {
    return m_Properties.keys();
}

const std::vector<Shader::Property>& Material::properties() const {
    return m_Properties.values();
}

bool Material::empty() const {
    return m_Properties.empty();
}

} // namespace Engine
