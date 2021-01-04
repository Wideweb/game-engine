#pragma once

#include "Application.hpp"
#include "Task.hpp"

namespace Engine {

class ControllerTask : public Task {
  private:
    KeyCode m_key;

  public:
    ControllerTask(KeyCode key) : m_key(key) {}

    TaskStatus update(std::shared_ptr<Blackboard> blackboard) override {
        auto &input = Application::get().getInput();
        return input.IsKeyPressed(m_key) ? TaskStatus::Success
                                         : TaskStatus::Fail;
    }
};

} // namespace Engine
