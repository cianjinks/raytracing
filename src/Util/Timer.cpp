#include "Timer.h"

namespace raytracing {

float Timer::s_ElapsedTime = 0;
bool Timer::s_MismatchFlag = false;
std::chrono::steady_clock::time_point Timer::s_Start =
    std::chrono::steady_clock::time_point::min();
std::chrono::steady_clock::time_point Timer::s_End =
    std::chrono::steady_clock::time_point::min();

void Timer::Start() {
    if (!s_MismatchFlag) {
        s_MismatchFlag = true;
        s_Start = std::chrono::steady_clock::now();
    } else {
        RT_ASSERT(false,
                  "Mismatch between Timer::Start() and Timer::End() calls");
    }
}

void Timer::End() {
    if (s_MismatchFlag) {
        s_MismatchFlag = false;

        s_End = std::chrono::steady_clock::now();
        s_ElapsedTime = std::chrono::duration<float>(s_End - s_Start).count();

    } else {
        RT_ASSERT(false,
                  "Mismatch between Timer::Start() and Timer::End() calls");
    }
}

float Timer::GetElapsedTimeMS() { return s_ElapsedTime * (1.0f / 1000.0f); }
float Timer::GetElapsedTimeS() { return s_ElapsedTime; }

}  // namespace raytracing