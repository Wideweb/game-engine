#pragma once

namespace Engine {

class Time {
  private:
    double m_deltaTime = 0;
    double m_totalTime = 0;
    double m_LastFrameTime = 0;

  public:
    void tick();

    double getDeltaSeconds() const { return m_deltaTime; }
    double getTotalSeconds() const { return m_totalTime; }
};

} // namespace Engine