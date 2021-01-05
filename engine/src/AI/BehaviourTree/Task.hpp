#pragma once

#include <memory>

#include "Blackboard.hpp"

namespace Engine {

enum class TaskStatus { Success = 0, Running = 1, Fail = 2 };

class Task {
  public:
    Task() {}
    virtual ~Task() {}

    virtual TaskStatus
    update(const std::shared_ptr<Blackboard> &blackboard = nullptr) = 0;
};

} // namespace Engine