#pragma once

namespace raytracing {

class Timer {
   private:
    float m_ElapsedTime;
    bool m_IsRunning;

    std::chrono::steady_clock::time_point m_Start;
    std::chrono::steady_clock::time_point m_End;

   public:
    Timer();
    ~Timer() = default;

    void Start();
    void Stop();

    bool IsRunning() const { return m_IsRunning; }

    float GetElapsedTimeMS() { return m_ElapsedTime * (1.0f / 1000.0f); }
    float GetElapsedTimeS() { return m_ElapsedTime; }
};

}  // namespace raytracing