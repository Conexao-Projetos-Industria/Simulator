cmake_minimum_required(VERSION 3.19.0)
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

find_package(Threads)
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
if (UNIX)
    find_package(glfw3 REQUIRED)
    find_package(GLEW REQUIRED)
endif ()

include(ExternalProject)

if (WIN32)
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
        # PREFIX ${PROJECT_SOURCE_DIR}/external/mujoco
        URL "https://github.com/deepmind/mujoco/releases/download/2.1.3/mujoco-2.1.3-windows-x86_64.zip"
        #URL_HASH SHA1=fe17a0610a239311a726ecabcd2dbd669fb24ca8
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND "" #${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/external/mujoco/src/external_mujoco/bin/mujoco.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/mujoco.dll 
    )
    file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/external/mujoco/include/)
    add_custom_command(
        TARGET external_mujoco POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_BINARY_DIR}/external_mujoco-prefix/src/external_mujoco/include/ ${PROJECT_SOURCE_DIR}/external/mujoco/include/
        COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_SOURCE_DIR}/external/mujoco/lib/
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_BINARY_DIR}/external_mujoco-prefix/src/external_mujoco/lib/ ${PROJECT_SOURCE_DIR}/external/mujoco/lib/
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/external_mujoco-prefix/src/external_mujoco/bin/mujoco.dll ${PROJECT_SOURCE_DIR}/external/mujoco/lib/
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/external_mujoco-prefix/src/external_mujoco/bin/mujoco_nogl.dll ${PROJECT_SOURCE_DIR}/external/mujoco/lib/
    )

    add_library(mujoco SHARED IMPORTED)
    add_dependencies(mujoco PUBLIC external_mujoco)
    target_include_directories(mujoco INTERFACE ${PROJECT_SOURCE_DIR}/external/mujoco/include)
    set_target_properties(mujoco PROPERTIES IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/external/mujoco/lib/mujoco.dll")
    set_target_properties(mujoco PROPERTIES IMPORTED_IMPLIB "${PROJECT_SOURCE_DIR}/external/mujoco/lib/mujoco.lib")
    target_link_libraries(mujoco INTERFACE glfw3 ${GLEW_LIBRARIES} ${OPENGL_gl_LIBRARY})

elseif (UNIX)
    ExternalProject_Add(
        external_mujoco
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/mujoco
        URL "https://github.com/deepmind/mujoco/releases/download/2.1.3/mujoco-2.1.3-linux-x86_64.tar.gz"
        #URL_HASH SHA1=fe17a0610a239311a726ecabcd2dbd669fb24ca8
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
    )

    file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/external/mujoco/include/)

    add_custom_command(
        TARGET external_mujoco POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/mujoco/src/external_mujoco/include/* ${PROJECT_SOURCE_DIR}/external/mujoco/include/
        COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_SOURCE_DIR}/external/mujoco/lib/
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/mujoco/src/external_mujoco/lib/* ${PROJECT_SOURCE_DIR}/external/mujoco/lib/
    )

    add_library(mujoco SHARED IMPORTED)
    add_dependencies(mujoco PUBLIC external_mujoco)
    target_include_directories(mujoco INTERFACE ${PROJECT_SOURCE_DIR}/external/mujoco/include)
    set_target_properties(mujoco PROPERTIES IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/external/mujoco/lib/libmujoco.so")
    target_link_libraries(mujoco INTERFACE glfw ${GLEW_LIBRARIES} ${OPENGL_gl_LIBRARY})
endif ()

add_subdirectory(source)
