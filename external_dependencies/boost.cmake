
#TODO try to find system boost first
#TODO boost is built every cmake run
ExternalProject_Add(boost_external
    GIT_REPOSITORY https://github.com/boostorg/boost
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_SOURCE_DIR}/external/boost/
    UPDATE_COMMAND ""
    # CONFIGURE_COMMAND ""
    UPDATE_DISCONNECTED 1
)

add_library(boost SHARED IMPORTED)
add_dependencies(boost PUBLIC boost_external)
target_include_directories(boost INTERFACE ${PROJECT_SOURCE_DIR}/external/boost/include/boost-1_79) #TODO boost version hardcoded
set_target_properties(boost PROPERTIES IMPORTED_IMPLIB "${PROJECT_SOURCE_DIR}/external/boost/lib/libboost_date_time-vc143-mt-gd-x64-1_79.lib") #TODO boost lib name hardcoded

set(BOOST_LIB boost)
