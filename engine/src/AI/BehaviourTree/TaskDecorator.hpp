#pragma once

#include "Task.hpp"

namespace Engine {

class TaskDecorator : public Task {
  protected:
    Task *m_Task;

  public:
    TaskDecorator(Task *task) : m_Task(task) {}

    virtual ~TaskDecorator() { delete m_Task; }
};

} // namespace Engine
