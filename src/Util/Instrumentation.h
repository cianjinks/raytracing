#pragma once

#include <tracy/Tracy.hpp>

namespace raytracing {

#ifdef RT_ENABLE_INSTRUMENTATION
#define RT_PROFILE_FUNC ZoneScoped
#define RT_PROFILE_FUNC_N(name) ZoneScopedN(name)
#else
#define RT_PROFILE_FUNC
#define RT_PROFILE_FUNC_N(name)
#endif

}  // namespace raytracing