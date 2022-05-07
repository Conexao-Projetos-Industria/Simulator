#include "server.h"
#include "server_http_session.h"

#include <iostream>

Server::Server(boost::asio::io_context& ioContext, boost::asio::ip::tcp::endpoint endpoint, const std::shared_ptr<ServerHandler>& state)
    : mSocket(ioContext)
    , mAcceptor(ioContext)
    , mState(state)
{
    boost::beast::error_code errorCode;

    this->mAcceptor.open(endpoint.protocol(), errorCode);
    if (errorCode) {
        this->Fail(errorCode, "Accepting connection.");
        return;
    }

    this->mAcceptor.set_option(boost::asio::socket_base::reuse_address(true));
    if (errorCode) {
        this->Fail(errorCode, "Setting connection option.");
        return;
    }

    this->mAcceptor.bind(endpoint, errorCode);
    if (errorCode) {
        this->Fail(errorCode, "Binding to endpoint.");
        return;
    }

    this->mAcceptor.listen(boost::asio::socket_base::max_listen_connections, errorCode);
    if (errorCode) {
        this->Fail(errorCode, "Listening to connections.");
        return;
    }
}

void Server::Run() {
    this->mAcceptor.async_accept(this->mSocket, [serverPtr = shared_from_this()](boost::beast::error_code errorCode)
    {
        serverPtr->OnAccept(errorCode);
    });
}

void Server::OnAccept(boost::beast::error_code errorCode) {
    if (errorCode) {
        this->Fail(errorCode, "Accepting connection.");
        return;
    }

    auto newSession = std::make_shared<ServerHttpSession>(std::move(this->mSocket), this->mState);
    newSession->Run();

    this->Run();
}

void Server::Fail(boost::beast::error_code errorCode, const char* when) {
    if(errorCode != boost::asio::error::operation_aborted) {
        std::cerr << "Error during: " << when << "  Error message is: " << errorCode.message() << "\n";
    }
}
