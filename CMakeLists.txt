cmake_minimum_required(VERSION 3.10)
project(CalculatorApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OpenGL_GL_PREFERENCE "GLVND")

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLU REQUIRED)

set(IMGUI_SOURCES
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_demo.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
   
) 

add_executable(Calculator main.cpp ${IMGUI_SOURCES})

target_include_directories(Calculator PUBLIC imgui imgui/backends)
target_link_libraries(Calculator glfw ${OpenGL_opengl_LIBRARY} ${OpenGL_glx_LIBRARY} GL X11)


