#pragma once
#include "server_handler.h"
// #include "beast.hpp"
// #include "net.hpp"
// #include <boost/smart_ptr.hpp>
#include <boost/beast/core.hpp>
#include <memory>
#include <string>

// Accepts incoming connections and launches the sessions
class Server : public std::enable_shared_from_this<Server>
{
    boost::asio::ip::tcp::acceptor mAcceptor;
    boost::asio::ip::tcp::socket mSocket;
    std::shared_ptr<ServerHandler> mState;

    static inline void CheckFail(boost::beast::error_code errorCode, const char* when);
    void Fail(boost::beast::error_code errorCode, const char* when);
    void OnAccept(boost::beast::error_code errorCode);

public:
    Server(
        boost::asio::io_context& ioContext,
        boost::asio::ip::tcp::endpoint endpoint,
        const std::shared_ptr<ServerHandler>& state);

    // Start accepting incoming connections
    void Run();
};
