cmake_minimum_required(VERSION 3.5)
project(raytracing-test)

enable_testing()

if (WIN32)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

add_subdirectory(googletest)

set(TEST_SOURCE
    "BBox.cpp"
)

add_executable(raytracing-test ${TEST_SOURCE} ${RT_SOURCES})
target_link_libraries(raytracing-test gtest_main ${RT_DEPENDENCIES})

target_precompile_headers(raytracing-test REUSE_FROM raytracing)

include(GoogleTest)
gtest_discover_tests(raytracing-test)