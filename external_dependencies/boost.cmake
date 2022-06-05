include(FetchContent)

FetchContent_Declare(
    boost
    GIT_REPOSITORY https://github.com/boostorg/boost.git
    GIT_TAG boost-1.79.0
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(boost)