cmake_minimum_required(VERSION 3.5)
project(raytracing)

# Options
option(RT_ENABLE_LOGGING "Enable logging via spdlog" ON)
option(RT_ENABLE_INSTRUMENTATION "Enable instrumentation via Tracy" OFF)
option(RT_ENABLE_TESTING "Enable building tests" ON)

# OpenGL
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
include_directories(src ${OPENGL_INCLUDE_DIRS})

# Address Sanitizer
if (UNIX)
	set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
	set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
endif()

# Define DEBUG and RELEASE flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DRT_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DRT_RELEASE")

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_USE_HYBRID_HPG ON CACHE BOOL "" FORCE)

# Dependencies
add_subdirectory(dependencies/glad)
add_subdirectory(dependencies/glfw)
add_subdirectory(dependencies/spdlog)
add_subdirectory(dependencies/stb_image)
add_subdirectory(dependencies/glm)
add_subdirectory(dependencies/tracy)
add_subdirectory(dependencies/nfd)
add_subdirectory(dependencies/imgui)
set(RT_DEPENDENCIES glad glfw stb_image glm spdlog TracyClient nfd imgui)

# Internal Sources
macro(rtSources name folder)
	file(GLOB_RECURSE ${name} CONFIGURE_DEPENDS "${folder}/*.h" "${folder}/*.cpp")
endmacro()

rtSources(RT_MATH_SRC src/Math)
rtSources(RT_RENDER_SRC src/Render)
rtSources(RT_UTIL_SRC src/Util)
rtSources(RT_WINDOW_SRC src/Window)
rtSources(RT_APP_SRC src/App)
set(RT_SOURCES ${RT_APP_SRC} ${RT_MATH_SRC} ${RT_RENDER_SRC} ${RT_UTIL_SRC} ${RT_WINDOW_SRC})

# Executable
add_executable(raytracing "src/Entry.cpp" ${RT_SOURCES})
target_link_libraries(raytracing PRIVATE ${OPENGL_LIBRARIES} ${RT_DEPENDENCIES})

# Evaluate Options
if (RT_ENABLE_LOGGING)
	add_compile_definitions(RT_ENABLE_LOGGING)
endif()
if (RT_ENABLE_INSTRUMENTATION)
	add_compile_definitions(RT_ENABLE_INSTRUMENTATION)
endif()

# PCH
target_precompile_headers(raytracing PRIVATE "src/PCH.h")

# Visual Studio Startup Project
if(WIN32)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT raytracing)
endif()

# Symlink Resources
add_custom_command(TARGET raytracing PRE_BUILD COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/resources $<TARGET_FILE_DIR:raytracing>/resources)

# Testing
if (RT_ENABLE_TESTING)
	add_subdirectory(test)
endif()