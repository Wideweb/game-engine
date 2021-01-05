#pragma once

#include "Task.hpp"

namespace Engine {

class NoopTask : public Task {
  public:
    NoopTask() {}

    TaskStatus update(const std::shared_ptr<Blackboard> &) override {
        return TaskStatus::Success;
    }
};

} // namespace Engine
