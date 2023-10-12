cmake_minimum_required(VERSION 3.5)
project(imgui)

set(imgui-source
    imconfig.h
	imgui.h
	imgui.cpp
	imgui_draw.cpp
	imgui_internal.h
	imgui_widgets.cpp
	imstb_rectpack.h
	imstb_textedit.h
	imstb_truetype.h
    imgui_tables.cpp
	imgui_demo.cpp
	backends/imgui_impl_glfw.cpp
	backends/imgui_impl_opengl3.cpp
)

add_library(imgui ${imgui-source})
target_link_libraries(imgui glfw)
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})