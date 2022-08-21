#pragma once

#ifdef RT_DEBUG
#define RT_ASSERT(condition, message)                                 \
    do {                                                              \
        if (!(condition)) {                                           \
            RT_ERROR("Assertion '{0}' failed at {1}:{2}", #condition, \
                     __FILE__, __LINE__);                             \
            std::terminate();                                         \
        }                                                             \
    } while (false)
#else
#define RT_ASSERT(condition, message) \
    do {                              \
    } while (false)
#endif