#pragma once

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace raytracing {

class Log {
   public:
    static void Init();
    inline static std::shared_ptr<spdlog::logger> &GetLogger() {
        return s_Logger;
    }

   private:
    static std::shared_ptr<spdlog::logger> s_Logger;
};

#ifdef RT_ENABLE_LOGGING
#define RT_LOG(...) ::raytracing::Log::GetLogger()->trace(__VA_ARGS__)
#define RT_WARN(...) ::raytracing::Log::GetLogger()->warn(__VA_ARGS__)
#define RT_ERROR(...) ::raytracing::Log::GetLogger()->error(__VA_ARGS__)
#else
#define RT_LOG(...)
#define RT_WARN(...)
#define RT_ERROR(...)
#endif

}  // namespace raytracing