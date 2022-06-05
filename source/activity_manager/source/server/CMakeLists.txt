add_library(Server STATIC source/server.cc)
target_include_directories(Server INTERFACE include)
target_include_directories(Server PRIVATE AFTER include private_include)
target_link_libraries(Server boost_beast ServerHttpSession)
