if (WIN32)

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
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/external_mujoco-prefix/src/external_mujoco/bin/mujoco.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/mujoco.dll
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/external_mujoco-prefix/src/external_mujoco/bin/mujoco_nogl.dll ${PROJECT_SOURCE_DIR}/external/mujoco/lib/
    )

    add_library(mujoco SHARED IMPORTED)
    add_dependencies(mujoco PUBLIC external_mujoco)
    target_include_directories(mujoco INTERFACE ${PROJECT_SOURCE_DIR}/external/mujoco/include)
    set_target_properties(mujoco PROPERTIES IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/external/mujoco/lib/mujoco.dll")
    set_target_properties(mujoco PROPERTIES IMPORTED_IMPLIB "${PROJECT_SOURCE_DIR}/external/mujoco/lib/mujoco.lib")
    target_link_libraries(mujoco INTERFACE ${GLFW3_LIB} ${GLEW_LIB} ${OPENGL_gl_LIBRARY})

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
    target_link_libraries(mujoco INTERFACE ${GLFW3_LIB} ${GLEW_LIB} ${OPENGL_gl_LIBRARY})

endif ()
