if (WIN32)
    add_library(position_info_channel STATIC source/shared_memory_channel_windows.cc)
    target_include_directories(position_info_channel INTERFACE include)
    target_include_directories(position_info_channel PRIVATE AFTER private_include)
    target_link_libraries(position_info_channel)

    add_library(position_info_channel_factory STATIC source/position_info_channel_factory.cc)
    target_include_directories(position_info_channel_factory INTERFACE include)
    target_include_directories(position_info_channel_factory PRIVATE AFTER private_include)
    target_link_libraries(position_info_channel_factory PRIVATE position_info_channel)
elseif(UNIX)

endif ()
