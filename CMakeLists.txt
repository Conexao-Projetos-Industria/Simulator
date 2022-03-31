cmake_minimum_required(VERSION 3.7.0)
project(Robot VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

if (WIN32)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib/our/windows)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib/our/windows)
elseif(UNIX)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib/our/linux)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib/our/linux)
endif ()

if (WIN32)
    message(STATUS "INCLUDING EXTERNAL GLFW3")

    include(ExternalProject)

    ExternalProject_Add(
        external_glfw3
        URL "https://github.com/glfw/glfw/archive/3.1.tar.gz"
        URL_HASH SHA1=fe17a0610a239311a726ecabcd2dbd669fb24ca8
        CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${PROJECT_SOURCE_DIR}/external/glfw3"
    )
    include_directories(${PROJECT_SOURCE_DIR}/external/glfw3/include)
    add_library(glfw3 STATIC IMPORTED)
    set_property(TARGET glfw3 PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/external/glfw3/lib/glfw3.lib)
    add_dependencies(glfw3 external_glfw3)

    ExternalProject_Add(
        external_glew
        URL "https://github.com/nigels-com/glew/releases/download/glew-2.2.0/glew-2.2.0.tgz"
        #URL_HASH SHA1=fe17a0610a239311a726ecabcd2dbd669fb24ca8
        SOURCE_SUBDIR "build/cmake/"
        CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${PROJECT_SOURCE_DIR}/external/glew"
    )
    include_directories(${PROJECT_SOURCE_DIR}/external/glew/include)
    add_library(glew STATIC IMPORTED)
    set_property(TARGET glew PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/external/glew/lib/glew32d.lib)
    add_dependencies(glew external_glew)

    ExternalProject_Add(
        external_mujoco
        PREFIX ${PROJECT_SOURCE_DIR}/external/downloaded/mujoco
        URL "https://github.com/deepmind/mujoco/releases/download/2.1.3/mujoco-2.1.3-windows-x86_64.zip"
        #URL_HASH SHA1=fe17a0610a239311a726ecabcd2dbd669fb24ca8
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/external/downloaded/mujoco/src/external_mujoco/bin/mujoco.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/mujoco.dll 
    )
    include_directories(${PROJECT_SOURCE_DIR}/external/mujoco/include)
    add_library(mujocoInstallDll STATIC IMPORTED)
    add_dependencies(mujocoInstallDll external_mujoco)

    list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
endif ()

find_package(Threads)
if (WIN32)
    set(OpenGL_GL_PREFERENCE GLVND)
    find_package(OpenGL REQUIRED)
elseif (UNIX)
    find_package(glfw3 REQUIRED)
    set(OpenGL_GL_PREFERENCE GLVND)
    find_package(OpenGL REQUIRED)
    find_package(GLEW REQUIRED)
endif ()

add_library(ui STATIC sample/uitools.c sample/uitools.h)

include_directories(include/)
if (WIN32)
    target_link_libraries(ui glfw3 glew ${OPENGL_LIBRARY})
    link_directories(${CMAKE_SOURCE_DIR}/lib/windows)
elseif (UNIX)
    target_link_libraries(ui glfw)
    link_directories(${CMAKE_SOURCE_DIR}/lib/linux)
endif ()

add_library(controller STATIC app/control.cpp)
add_executable(sim_base app/sim_base.cpp)
if (WIN32)
    target_link_libraries(sim_base controller ui mujoco ${CMAKE_THREAD_LIBS_INIT} glfw3)
elseif (UNIX)
    target_link_libraries(sim_base controller ui mujoco ${CMAKE_THREAD_LIBS_INIT} glfw ${GLEW_LIBRARIES} ${OPENGL_gl_LIBRARY})
endif ()