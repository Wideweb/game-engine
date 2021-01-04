#pragma once

#include "TaskDecorator.hpp"

namespace Engine {

class BlackboardManager : public TaskDecorator {
  private:
    std::shared_ptr<Blackboard> m_Blackboard;

  public:
    BlackboardManager(std::shared_ptr<Blackboard> blackboard, Task *task)
        : TaskDecorator(task), m_Blackboard(blackboard) {}

    virtual TaskStatus update(std::shared_ptr<Blackboard> blackboard) override {
        if (blackboard) {
            m_Blackboard->setParent(blackboard);
        }

        return m_Task->update(m_Blackboard);
    }
};

} // namespace Engine
