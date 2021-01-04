#pragma once

#include <memory>

#include "Task.hpp"

namespace Engine {

class AIComponent {
  public:
    std::shared_ptr<Task> tree;

    AIComponent() {}
    AIComponent(std::shared_ptr<Task> tree) : tree(tree) {}
};

} // namespace Engine