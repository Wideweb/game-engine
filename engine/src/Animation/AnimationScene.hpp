#pragma once

#include "TimeLine.hpp"

#include <vector>

namespace Engine {

class AnimationScene {
  private:
    std::vector<TimeLine> m_TimeLines;

  public:
    AnimationScene() {}
    AnimationScene(const std::vector<TimeLine> &timeLines)
        : m_TimeLines(timeLines) {}

    void add(const TimeLine &timeLine) { m_TimeLines.push_back(timeLine); }

    std::vector<TimeLineStep> step(float deltaTime) {
        std::vector<TimeLineStep> steps;

        for (TimeLine &timeLine : m_TimeLines) {
            if (!timeLine.isEnd()) {
                steps.push_back(timeLine.step(deltaTime));
            }
        }

        return steps;
    }

    void reset() {
        for (TimeLine &timeLine : m_TimeLines) {
            timeLine.reset();
        }
    }
};

} // namespace Engine