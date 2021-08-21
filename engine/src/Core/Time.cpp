#include "Time.hpp"

#include <chrono>

namespace Engine {

void Time::tick() {
    auto time_point = std::chrono::high_resolution_clock::now();
    auto time_since_epoch = time_point.time_since_epoch();
    auto ns = time_since_epoch.count();
    double seconds = static_cast<double>(ns) / 1000'000'000.0f;

    if (m_Stop) {
        m_LastFrameTime = seconds;
    }

    m_deltaTime = seconds - m_LastFrameTime;
    m_totalTime += m_deltaTime;
    m_LastFrameTime = seconds;
}

void Time::play() { m_Stop = false; }

void Time::stop() {
    m_Stop = true;
    m_deltaTime = 0;
}

} // namespace Engine