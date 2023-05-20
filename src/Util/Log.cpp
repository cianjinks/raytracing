#include "Log.h"

namespace raytracing {

std::shared_ptr<spdlog::logger> Log::s_Logger;

void Log::Init() {
    RT_PROFILE_FUNC;
    s_Logger = spdlog::stdout_color_mt("raytracing");
    s_Logger->set_level(spdlog::level::trace);
    spdlog::set_pattern("%^[%T][%n] %v%$");
    RT_LOG("Logger Initialised");
}

}  // namespace raytracing