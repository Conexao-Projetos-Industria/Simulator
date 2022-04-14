if (WIN32)
    find_package(OpenSSL)

    add_library(activity_manager_connector STATIC source/activity_manager_connector.cc)
    target_include_directories(activity_manager_connector INTERFACE include)
    target_include_directories(activity_manager_connector PRIVATE AFTER private_include include)
    target_link_libraries(activity_manager_connector ${BOOST_LIB}  OpenSSL::OpenSSL)

    add_library(activity_manager_connector_factory STATIC source/activity_manager_connector_factory.cc)
    target_include_directories(activity_manager_connector_factory INTERFACE include)
    target_include_directories(activity_manager_connector_factory PRIVATE AFTER private_include)
    target_link_libraries(activity_manager_connector_factory PRIVATE activity_manager_connector)
elseif(UNIX)

endif ()
