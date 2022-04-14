if (WIN32)

    ExternalProject_Add(
        external_glew
        URL "https://github.com/nigels-com/glew/releases/download/glew-2.2.0/glew-2.2.0.tgz"
        #URL_HASH SHA1=fe17a0610a239311a726ecabcd2dbd669fb24ca8
        SOURCE_SUBDIR "build/cmake/"
        CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${PROJECT_SOURCE_DIR}/external/glew"
    )
    include_directories(${PROJECT_SOURCE_DIR}/external/glew/include)
    add_library(GLEW STATIC IMPORTED)
    set_property(TARGET GLEW PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/external/glew/lib/glew32d.lib)
    add_dependencies(GLEW external_glew)

elseif(UNIX)

    find_package(GLEW REQUIRED)
    
endif ()

set(GLEW_LIB GLEW)
