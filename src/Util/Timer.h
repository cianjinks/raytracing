#pragma once

namespace raytracing {

class Timer {
   private:
    static float s_ElapsedTime;
    static bool s_MismatchFlag; /* Track if there is a mismatch between calling
                                   Start() and End(). */

    static std::chrono::_V2::system_clock::time_point s_Start;
    static std::chrono::_V2::system_clock::time_point s_End;

   public:
    static void Start();
    static void End();
    static float GetElapsedTimeMS();
    static float GetElapsedTimeS();
};

}  // namespace raytracing