cmake_minimum_required(VERSION 3.0.0)
project(Robot VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

if (WIN32)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib/our/windows)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib/our/windows)
elseif(UNIX)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib/our/linux)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib/our/linux)
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

add_library(ui STATIC sample/uitools.c sample/uitools.h)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package( Threads )

include_directories(include/)

if (WIN32)
    link_directories(${CMAKE_SOURCE_DIR}/lib/linux)
elseif(UNIX)
    link_directories(${CMAKE_SOURCE_DIR}/lib/linux)
endif ()

add_library(controller STATIC app/control.cpp)

add_executable(sim_base app/sim_base.cpp)
target_link_libraries(sim_base controller ui mujoco ${CMAKE_THREAD_LIBS_INIT} glfw ${GLEW_LIBRARIES} ${OPENGL_gl_LIBRARY})
