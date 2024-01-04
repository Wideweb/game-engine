#include "GLSLPreprocessor.hpp"

#include "File.hpp"
#include "StringBuilder.hpp"

#include <fstream>
#include <iostream>
#include <iterator>
#include <regex>
#include <stdexcept>

namespace Engine {

GlMetaData GLSLPreprocessor::preprocess(const std::string &path, const std::string &srcDir) {
    std::string sourceCode = File::read(path);

    GlMetaData metaData;

    try {
        sourceCode = parseIncludes(sourceCode, srcDir);
        sourceCode = parseStructDeclarations(sourceCode, metaData);
        sourceCode = parseUniforms(sourceCode, metaData);
    } catch (std::string err) {
        return GlMetaData::makeError(err);
    }

    metaData.sourceCode = sourceCode;

    return metaData;
}

std::string GLSLPreprocessor::parseIncludes(std::string sourceCode, const std::string &srcDir) {
    std::regex includePattern("#include\\s+\"(.+)\"");
    std::smatch matches;

    while (std::regex_search(sourceCode, matches, includePattern)) {
        if (matches.size() < 2) {
            std::string message = StringBuilder() << "Invalid shader #include: " << matches[0].str();
            throw message;
        }

        std::string includePath = srcDir + matches[1].str();
        std::string srcToInclude = File::read(includePath);

        sourceCode.replace(matches.position(), matches[0].length(), srcToInclude);
    }

    return sourceCode;
}

std::string GLSLPreprocessor::parseUniforms(std::string sourceCode, GlMetaData &metaData) {
    std::regex uniformPattern("(\\s*\\@\\w+(\\(.*?\\))?\n?)*\\s*uniform\\s*(\\w+)\\s+(\\w+);");
    std::string preprocessedCode = sourceCode;

    auto uniformsBegin = std::sregex_iterator(sourceCode.begin(), sourceCode.end(), uniformPattern);
    auto uniformsEnd = std::sregex_iterator();

    for (std::sregex_iterator i = uniformsBegin; i != uniformsEnd; ++i) {
        std::smatch match = *i;
        std::string fieldName = match[4].str();
        std::string fieldType = match[3].str();
        std::string fieldCode = match[0].str();

        auto propertyMetaData = std::make_unique<GLPropertyMetaData>(fieldName, fieldType);
        preprocessedCode.replace(preprocessedCode.find(fieldCode), fieldCode.length(),
                                 parsePropertyAttributes(fieldCode, *propertyMetaData));
        metaData.uniforms.add(fieldName, std::move(propertyMetaData));
    }

    return preprocessedCode;
}

std::string GLSLPreprocessor::parseStructDeclarations(std::string sourceCode, GlMetaData &metaData) {
    std::regex structPattern("struct\\s+(\\w+)\\s*\\{(\n*.*\n*)+?\\};");
    std::string preprocessedCode = sourceCode;

    auto structBegin = std::sregex_iterator(sourceCode.begin(), sourceCode.end(), structPattern);
    auto structEnd = std::sregex_iterator();

    for (std::sregex_iterator i = structBegin; i != structEnd; ++i) {
        std::smatch match = *i;
        std::string structName = match[1].str();
        std::string structCode = match[0].str();

        GlStructMetaData structMetaData{.name = structName};
        preprocessedCode.replace(preprocessedCode.find(structCode), structCode.length(),
                                 parseStruct(structCode, structMetaData));
        metaData.structs.add(structName, std::move(structMetaData));
    }

    return preprocessedCode;
}

std::string GLSLPreprocessor::parseStruct(std::string structCode, GlStructMetaData &structMetaData) {
    std::regex fieldPattern("(\\s*\\@\\w+(\\(.*?\\))?\n)*\\s*(\\w+)\\s+(\\w+);");
    std::string preprocessedCode = structCode;

    auto fieldsBegin = std::sregex_iterator(structCode.begin(), structCode.end(), fieldPattern);
    auto fieldsEnd = std::sregex_iterator();

    for (std::sregex_iterator i = fieldsBegin; i != fieldsEnd; ++i) {
        std::smatch match = *i;
        std::string fieldName = match[4].str();
        std::string fieldType = match[3].str();
        std::string fieldCode = match[0].str();

        auto propertyMetaData = std::make_unique<GLPropertyMetaData>(fieldName, fieldType);
        preprocessedCode.replace(preprocessedCode.find(fieldCode), fieldCode.length(),
                                 parsePropertyAttributes(fieldCode, *propertyMetaData));
        structMetaData.fields.add(fieldName, std::move(propertyMetaData));
    }

    return preprocessedCode;
}

std::string GLSLPreprocessor::parsePropertyAttributes(std::string fiedlDeclarationCode,
                                                      GLPropertyMetaData &propertyMetaData) {
    std::regex attributePattern("\\@(\\w+)(\\(.*?\\))?\\s*");
    std::smatch match;

    while (std::regex_search(fiedlDeclarationCode, match, attributePattern)) {
        std::string name = match[1].str();
        std::string params = match[2].str();

        if (name == "Editable") {
            propertyMetaData.addAttribute(parseEditableAttribute(params));
        } else if (name == "Color") {
            propertyMetaData.addAttribute(parseColorAttribute(params));
        } else if (name == "Bool") {
            propertyMetaData.addAttribute(parseBoolAttribute(params));
        } else if (name == "Range") {
            propertyMetaData.addAttribute(parseRangeAttribute(params));
        }

        fiedlDeclarationCode.erase(match.position(), match[0].length());
    }

    return fiedlDeclarationCode;
}

GLEditableAttribute *GLSLPreprocessor::parseEditableAttribute(std::string &paramsCode) {
    std::regex paramsPattern("\"(.+?)\"");
    std::smatch matches;
    std::regex_search(paramsCode, matches, paramsPattern);

    if (matches.size() < 2) {
        std::cerr << "Invalid shader editable attribute params: " << paramsCode << "\n";
        return nullptr;
    }

    std::string label = matches[1].str();
    return new GLEditableAttribute(label);
}

GLColorAttribute *GLSLPreprocessor::parseColorAttribute(std::string &paramsCode) { return new GLColorAttribute(); }

GLBoolAttribute *GLSLPreprocessor::parseBoolAttribute(std::string &paramsCode) { return new GLBoolAttribute(); }

GLRangeAttribute *GLSLPreprocessor::parseRangeAttribute(std::string &paramsCode) {
    std::regex paramsPattern("[+-]?([0-9]*[.])?[0-9]+");
    std::smatch match;
    auto paramsPtr = std::sregex_iterator(paramsCode.begin(), paramsCode.end(), paramsPattern);
    auto paramsEnd = std::sregex_iterator();

    if (paramsPtr == paramsEnd) {
        std::cerr << "Invalid shader range attribute params: " << paramsCode << "\n";
        return nullptr;
    }

    std::string fromStr = (*paramsPtr).str();

    paramsPtr++;
    if (paramsPtr == paramsEnd) {
        std::cerr << "Invalid shader range attribute params: " << paramsCode << "\n";
        return nullptr;
    }

    std::string toStr = (*paramsPtr).str();

    float from = std::stof(fromStr);
    float to = std::stof(toStr);

    return new GLRangeAttribute(from, to);
}

} // namespace Engine
