find_package(Threads)
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

include(external_dependencies/openssl.cmake)
include(external_dependencies/glfw3.cmake)
include(external_dependencies/boost.cmake)
include(external_dependencies/mujoco.cmake)
# include(external_dependencies/lcm.cmake) #TODO change from websockets to lcm(udp multicast). Need instalation of lib GLib2 for windows. lcm provides future connection with drake. lcm is MIT+Toyota tested.
