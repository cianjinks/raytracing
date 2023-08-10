#pragma once

#include <tracy/Tracy.hpp>

#include <glad/glad.h>
#include <tracy/TracyOpenGL.hpp>

namespace raytracing {

#ifdef RT_ENABLE_INSTRUMENTATION
#define RT_PROFILE_FUNC ZoneScoped
#define RT_PROFILE_FUNC_N(name) ZoneScopedN(name)
#define RT_PROFILE_THREAD_N(name) tracy::SetThreadName(name)
#define RT_PROFILE_GPU_CONTEXT TracyGpuContext
#define RT_PROFILE_GPU_SWAP TracyGpuCollect; FrameMark
#else
#define RT_PROFILE_FUNC
#define RT_PROFILE_FUNC_N(name)
#define RT_PROFILE_GPU_CONTEXT
#define RT_PROFILE_GPU_SWAP
#endif

}  // namespace raytracing