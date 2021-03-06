#pragma once

#include <memory>
#include <string>

namespace Engine {

class BehaviourComponent {
  public:
    std::string script;

    BehaviourComponent() {}
    BehaviourComponent(std::string script) : script(std::move(script)) {}
};

} // namespace Engine