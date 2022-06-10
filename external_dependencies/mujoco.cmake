include(FetchContent)

FetchContent_Declare(
    mujoco
    GIT_REPOSITORY https://github.com/deepmind/mujoco.git
    GIT_TAG main
)

FetchContent_GetProperties(mujoco)
if(NOT mujoco_POPULATED)
    FetchContent_Populate(mujoco)
    set(MUJOCO_BUILD_EXAMPLES OFF)
    set(MUJOCO_BUILD_TESTS OFF)
    set(MUJOCO_TEST_PYTHON_UTIL OFF)
    # Prevent targets from being added to the ALL build. As a result only dependencies referenced
    # in other targets will be built. INSTALL target will not be built for example.
    add_subdirectory(${mujoco_SOURCE_DIR} ${mujoco_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()