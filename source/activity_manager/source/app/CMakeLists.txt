add_executable(activity_manager activity_manager.cc)

# target_include_directories(activity_manager PUBLIC include)

find_package(OpenSSL)
target_link_libraries(activity_manager ${CMAKE_THREAD_LIBS_INIT} boost_beast OpenSSL::SSL Server ServerHandler)
