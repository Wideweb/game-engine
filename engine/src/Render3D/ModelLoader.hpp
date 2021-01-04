#pragma once

#include <memory>
#include <sstream>
#include <string>

#include "Model.hpp"

namespace Engine {

class ModelLoader {
  public:
    static std::shared_ptr<Model> load(const std::string &toObj,
                                       const std::string &toDiffuseMap,
                                       const std::string &toSpecularMap,
                                       const std::string &toNormalMap);
};

} // namespace Engine
