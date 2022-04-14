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

elseif(UNIX)

    # find_package(glfw3 REQUIRED)
    
endif ()

set(GLFW3_LIB glfw3)
