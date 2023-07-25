#include "Timer.h"

namespace raytracing {

Timer::Timer() {
    m_ElapsedTime = 0.0f;
    m_IsRunning = false;
    m_Start = std::chrono::steady_clock::time_point::min();
    m_End = std::chrono::steady_clock::time_point::min();
}

void Timer::Start() {
    if (!m_IsRunning) {
        m_IsRunning = true;
        m_Start = std::chrono::steady_clock::now();
    } else {
        RT_ASSERT(false,
                  "Mismatch between Timer::Start() and Timer::End() calls");
    }
}

void Timer::Stop() {
    if (m_IsRunning) {
        m_IsRunning = false;

        m_End = std::chrono::steady_clock::now();
        m_ElapsedTime = std::chrono::duration<float>(m_End - m_Start).count();

    } else {
        RT_ASSERT(false,
                  "Mismatch between Timer::Start() and Timer::End() calls");
    }
}

}  // namespace raytracing