#pragma once

#include <tracy/Tracy.hpp>

namespace raytracing {

#ifdef RT_ENABLE_INSTRUMENTATION
#define RT_PROFILE_FUNC ZoneScoped
#define RT_PROFILE_FUNC_N(name) ZoneScopedN(name)
#define RT_PROFILE_THREAD_N(name) tracy::SetThreadName(name)
#define RT_PROFILE_GPU_SWAP FrameMark
#else
#define RT_PROFILE_FUNC
#define RT_PROFILE_FUNC_N(name)
#define RT_PROFILE_GPU_SWAP
#endif

}  // namespace raytracing