find_package(Threads)
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

include(ExternalProject)

if(USING_CONAN)

    include(external_dependencies/run_conan.cmake)

    find_package(GLEW)
    set(GLEW_LIB GLEW::GLEW)

    find_package(glfw3)
    set(GLFW3_LIB glfw)

    find_package(Boost)
    if (WIN32)
        set(BOOST_LIB Boost::boost)
    elseif (UNIX)
        set(BOOST_LIB Boost::boost rt)
    endif ()
else()

    include(external_dependencies/glew.cmake)
    include(external_dependencies/glfw3.cmake)
    include(external_dependencies/boost.cmake)

endif()

include(external_dependencies/mujoco.cmake)
# include(external_dependencies/lcm.cmake) #TODO change from websockets to lcm(udp multicast). Need instalation of lib GLib2 for windows. lcm provides future connection with drake. lcm is MIT+Toyota tested.
