#include "File.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <regex>
#include <iterator>

namespace Engine {

std::string File::read(const std::string &path) {
    std::string result;
    std::ifstream in(path, std::ios_base::binary);

    if (!in) {
        return "error";
    }

    in.seekg(0, std::ios_base::end);
    auto size = in.tellg();
    result.resize(static_cast<size_t>(size));
    in.seekg(0, std::ios_base::beg);
    in.read(&result[0], size);
    in.close();

    if (!in) {
        return "error";
    }

    if (!in.good()) {
        return "error";
    }

    return result;
}

std::string File::readGLSL(const std::string& path, const std::string& srcDir) {
    std::string sourceCode = File::read(path);

    std::regex includePattern("#include\\s+\"(.+)\"");
    std::smatch matches;

    while (std::regex_search(sourceCode, matches, includePattern)) {
        if (matches.size() < 2) {
            std::cerr << "Invalid shader #include: " << matches[0].str() << "\n";
            return "";
        }

        std::string includePath = srcDir + matches[1].str();
        std::string srcToInclude = File::read(includePath);

        sourceCode.replace(matches.position(), matches[0].length(), srcToInclude);
    }

    return sourceCode;
}

std::string File::extension(const std::string &path) { return path.substr(path.find_last_of(".") + 1); }
std::string File::name(const std::string &path) { return path.substr(path.find_last_of("/") + 1); }

} // namespace Engine
