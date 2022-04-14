#LINUX
#sudo apt install libglib2.0-dev -y


#WINDOWS
#

ExternalProject_Add(
    lcm_external
    URL "https://github.com/lcm-proj/lcm/releases/download/v1.4.0/lcm-1.4.0.zip"
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_SOURCE_DIR}/external/boost/
)

add_library(lcm STATIC IMPORTED)
add_dependencies(lcm lcm_external)
