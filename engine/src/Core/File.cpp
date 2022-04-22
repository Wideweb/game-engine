#include "File.hpp"

#include <fstream>
#include <iostream>

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

std::string File::extension(const std::string &path) { return path.substr(path.find_last_of(".") + 1); }
std::string File::name(const std::string &path) { return path.substr(path.find_last_of("/") + 1); }

} // namespace Engine
