#pragma once

namespace Engine {

class Time {
  private:
    float m_deltaTime = 0;
    float m_totalTime = 0;
    float m_LastFrameTime = 0;

  public:
    void tick();

    float getDeltaSeconds() const { return m_deltaTime; }
    float getTotalSeconds() const { return m_totalTime; }
};

} // namespace Engine