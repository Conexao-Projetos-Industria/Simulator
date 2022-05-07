#pragma once
#include "server_handler.h"

#include <memory>
#include <boost/beast/core.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/beast/http/string_body.hpp>

class ServerHttpSession : public std::enable_shared_from_this<ServerHttpSession> {
    boost::asio::ip::tcp::socket mSocket;
    std::shared_ptr<ServerHandler> mState;
    boost::beast::flat_buffer mBuffer; //(wrapper to a data pointer + length)
    boost::beast::http::request<boost::beast::http::string_body> mRequest;

    void OnRead(boost::beast::error_code error_code, std::size_t bytes);
    void OnWrite(boost::beast::error_code errorCode, std::size_t, bool close);
    void Fail(boost::beast::error_code errorCode, char const* when);
    public:
        ServerHttpSession(boost::asio::ip::tcp::socket socket, const std::shared_ptr<ServerHandler>& state);
        void Run();
};
