#pragma once

namespace Engine {

class Time {
  private:
    double m_deltaTime = 0;
    double m_totalTime = 0;
    double m_LastFrameTime = 0;
    float m_Stop = false;

  public:
    void tick();
    void play();
    void stop();

    bool poused() { return m_Stop; }
    double getDeltaSeconds() const { return m_deltaTime; }
    double getTotalSeconds() const { return m_totalTime; }
};

} // namespace Engine