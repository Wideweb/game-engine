#pragma once

#include "FlatDictionary.hpp"
#include "GLMetaData.hpp"

#include <string>
#include <vector>

namespace Engine {

class GLSLPreprocessor {
  public:
    static GlMetaData preprocess(const std::string &path, const std::string &srcDir = "./shaders/");

  private:
    static std::string parseIncludes(std::string sourceCode, const std::string &srcDir);
    static std::string parseUniforms(std::string sourceCode, GlMetaData &metaData);
    static std::string parseStructDeclarations(std::string sourceCode, GlMetaData &metaData);
    static std::string parseStruct(std::string structCode, GlStructMetaData &structMetaData);
    static std::string parsePropertyAttributes(std::string propertyDeclarationCode,
                                               GLPropertyMetaData &propertyMetaData);
    static GLEditableAttribute *parseEditableAttribute(std::string &paramsCode);
    static GLColorAttribute *parseColorAttribute(std::string &paramsCode);
    static GLBoolAttribute *parseBoolAttribute(std::string &paramsCode);
    static GLRangeAttribute *parseRangeAttribute(std::string &paramsCode);
};

} // namespace Engine
