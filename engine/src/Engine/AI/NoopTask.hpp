#pragma once

#include "Task.hpp"

namespace Engine {

class NoopTask : public Task {
  public:
    NoopTask() {}

    TaskStatus update(std::shared_ptr<Blackboard> blackboard) override {
        return TaskStatus::Success;
    }
};

} // namespace Engine
