#pragma once

#include "server_handler.h"

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <memory>

using tcp = boost::asio::ip::tcp;
namespace http = boost::beast::http;
namespace websocket = boost::beast::websocket;

using basicMessageType = boost::beast::http::message<0,struct boost::beast::http::basic_string_body<char,struct std::char_traits<char>,class std::allocator<char> >,class boost::beast::http::basic_fields<class std::allocator<char> > >;

// class ServerState;
class ServerWebsocktSession : public std::enable_shared_from_this<ServerWebsocktSession>{
    websocket::stream<tcp::socket> mWebsocketStream;
    std::shared_ptr<ServerHandler> mState;
    boost::beast::flat_buffer mBuffer;

    std::vector<std::shared_ptr<std::string const>> mOutQueue;

    void OnAccept(boost::beast::error_code errorCode);
    void OnRead(boost::beast::error_code errorCode, std::size_t bytesRead);
    void OnSend(std::shared_ptr<std::string const> const& message);
    void OnWrite(boost::beast::error_code errorCode, std::size_t sentSize);
    public:
        ServerWebsocktSession(tcp::socket socket, const std::shared_ptr<ServerHandler>& state);
        ~ServerWebsocktSession();

        template<class Body, class Allocator>
        void Start(boost::beast::http::request<Body, boost::beast::http::basic_fields<Allocator>> request);
        void Send(std::shared_ptr<std::string const> const& message);
        void Fail(boost::beast::error_code errorCode, char const* when);
};

template<class Body, class Allocator>
void ServerWebsocktSession::Start(http::request<Body, http::basic_fields<Allocator>> req) {
    mWebsocketStream.async_accept(req, [websocketPtr = shared_from_this()](boost::beast::error_code errorCode) {
        websocketPtr->OnAccept(errorCode);
    });
}
