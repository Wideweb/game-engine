#pragma once

#include "FlatDictionary.hpp"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace Engine {

struct GLAttribute {};

struct GLEditableAttribute : GLAttribute {
    std::string label;

    GLEditableAttribute(const std::string &label) : label(label) {}
};

struct GLRangeAttribute : GLAttribute {
    float from;
    float to;

    GLRangeAttribute(float from, float to) : from(from), to(to) {}
};

struct GLColorAttribute : GLAttribute {};

struct GLBoolAttribute : GLAttribute {};

class GLPropertyMetaData {
  public:
    std::string name;
    std::string type;

    GLPropertyMetaData(const std::string name, const std::string type) : name(name), type(type) {}

    template <typename T> T *getAttribute() {
        const auto *key = &typeid(T);
        auto it = m_attributes.find(key);
        if (it == m_attributes.end()) {
            return nullptr;
        }
        return (T *)(*it);
    }

    template <typename T> void addAttribute(T *attribute) {
        const auto *key = &typeid(T);
        m_attributes.emplace(key, std::unique_ptr<T>(attribute));
    }

    template <typename T> bool hasAttribute() {
        const auto *key = &typeid(T);
        return m_attributes.find(key) != m_attributes.end();
    }

  private:
    std::unordered_map<const std::type_info *, std::unique_ptr<GLAttribute>> m_attributes;
};

struct GlStructMetaData {
    std::string name;
    FlatDictionary<std::string, std::unique_ptr<GLPropertyMetaData>> fields;
};

struct GlMetaData {
    FlatDictionary<std::string, GlStructMetaData> structs;
    FlatDictionary<std::string, std::unique_ptr<GLPropertyMetaData>> uniforms;
    std::string sourceCode;
    std::string error;

    static GlMetaData makeError(const std::string message) { return {.error = message}; }
};

} // namespace Engine
