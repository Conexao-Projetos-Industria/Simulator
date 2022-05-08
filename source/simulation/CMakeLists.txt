add_executable(simulator source/sim_base.cc)
if (WIN32)
    target_link_libraries(simulator mujoco ${CMAKE_THREAD_LIBS_INIT} ${GLFW3_LIB} sim_controller_factory)
    # Mujoco.dll is copied at this point to prevent problems with CMAKE_RUNTIME_OUTPUT_DIRECTORY value changing during build
    add_custom_command(
        TARGET simulator POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/external/mujoco/lib/mujoco.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/mujoco.dll
    )
elseif (UNIX)
    target_link_libraries(simulator mujoco ${CMAKE_THREAD_LIBS_INIT} sim_controller_factory librt.so)
endif ()
