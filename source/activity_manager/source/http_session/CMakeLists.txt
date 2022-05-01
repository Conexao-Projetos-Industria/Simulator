add_library(HttpSession STATIC source/http_session.cc)
target_include_directories(HttpSession INTERFACE include)
target_include_directories(HttpSession PRIVATE AFTER include private_include)
target_link_libraries(HttpSession ${BOOST_LIB} ServerHandler WebsocketSession)
