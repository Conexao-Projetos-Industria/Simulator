#pragma once

#include "server_handler.h"

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <memory>

using tcp = boost::asio::ip::tcp;
namespace http = boost::beast::http;
namespace websocket = boost::beast::websocket;

// class ServerState;
class WebsocktSession : public std::enable_shared_from_this<WebsocktSession>{
    websocket::stream<tcp::socket> mWebsocketStream;
    std::shared_ptr<ServerHandler> mState;
    boost::beast::flat_buffer mBuffer;

    public:
        WebsocktSession(tcp::socket socket, const std::shared_ptr<ServerHandler>& state);
        ~WebsocktSession();

        template<class Body, class Allocator>
        void Start(boost::beast::http::request<Body, boost::beast::http::basic_fields<Allocator>> request);

        void OnAccept(boost::beast::error_code errorCode);
        void OnRead(boost::beast::error_code errorCode, std::size_t bytesRead);
        void Fail(boost::beast::error_code errorCode, char const* when);
};

template<class Body, class Allocator>
void WebsocktSession::Start(http::request<Body, http::basic_fields<Allocator>> req) {
    mWebsocketStream.async_accept(req, [websocketPtr = shared_from_this()](boost::beast::error_code errorCode) {
        websocketPtr->OnAccept(errorCode);
    });
}
