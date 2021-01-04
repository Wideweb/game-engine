#pragma once

#include "Animation.hpp"

#include <vector>

namespace Engine {

class TimeLine {
  private:
    int m_ActionIndex = 0;
    std::vector<Action> m_Actions;
    float m_ElapsedTime = 0.0;
    float m_Step = 0.0;
    bool m_Looped = false;

  public:
    enum class Properties { Once, Looped };

    TimeLine() {}
    TimeLine(Properties prop) { m_Looped = prop == Properties::Looped; }
    TimeLine(const std::vector<Action> &actions, Properties prop)
        : m_Actions(actions) {
        m_Looped = prop == Properties::Looped;
    }

    void add(const Action &action) { m_Actions.push_back(action); }

    TimeLineStep step(float deltaTime) {
        Action &action = getCurrent();

        if (action.type == ActionType::PlayMusic) {
            auto step = TimeLineStep::playSound(action.sound, action.volume);

            next();
            return step;
        }

        if (action.type == ActionType::Active) {
            auto step = TimeLineStep::active(action.component);

            next();
            return step;
        }

        if (action.type == ActionType::Inactive) {
            auto step = TimeLineStep::inactive(action.component);

            next();
            return step;
        }

        m_ElapsedTime += deltaTime;
        if (m_ElapsedTime > action.time) {
            auto step = TimeLineStep::animation(
                action.property, action.targetValue - action.currentValue);
            next();
            return step;
        }

        float t = m_ElapsedTime / action.time;
        float value = action.targetValue * t;

        m_Step = value - action.currentValue;
        action.currentValue = value;

        return TimeLineStep::animation(action.property, m_Step);
    }

    Action &getCurrent() { return m_Actions[m_ActionIndex]; }

    bool isEnd() { return m_ActionIndex == m_Actions.size(); }

    void reset() { m_ActionIndex = 0; }

  private:
    void next() {
        Action &action = getCurrent();
        action.currentValue = 0;

        // m_ElapsedTime -= animation.time;
        m_ElapsedTime = 0;
        m_Step = 0.0;
        m_ActionIndex++;
        if (m_ActionIndex == m_Actions.size() && m_Looped) {
            m_ActionIndex = 0;
        }
    }
};

} // namespace Engine