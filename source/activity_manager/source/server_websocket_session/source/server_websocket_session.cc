#include "server_websocket_session.h"

#include <iostream>

ServerWebsocktSession::ServerWebsocktSession(tcp::socket socket, const std::shared_ptr<ServerHandler>& state)
    : mWebsocketStream(std::move(socket))
    , mState(state)
{}

ServerWebsocktSession::~ServerWebsocktSession()
{
    this->mState->Leave(this);
}

void ServerWebsocktSession::OnAccept(boost::beast::error_code errorCode) {
    if(errorCode) {
        this->Fail(errorCode, "Accepting");
        return;
    }

    this->mState->Add(this);

    this->mWebsocketStream.async_read(this->mBuffer, [sessionPtr = shared_from_this()](boost::beast::error_code errorCode, std::size_t bytesRead) {
        sessionPtr->OnRead(errorCode, bytesRead);
    });
}

void ServerWebsocktSession::OnRead(boost::beast::error_code errorCode, std::size_t bytesRead) {
    if(errorCode) {
        this->Fail(errorCode, "Reading");
        return;
    }

    std::cout << "[ServerWebsocktSession] bytes read " << bytesRead << std::endl;
    this->mState->HandleWebsocketMessage(this, boost::beast::buffers_to_string(this->mBuffer.data()));

    this->mBuffer.consume(this->mBuffer.size());

    this->mWebsocketStream.async_read(this->mBuffer, [sessionPtr = shared_from_this()](boost::beast::error_code errorCode, std::size_t bytesRead) {
        sessionPtr->OnRead(errorCode, bytesRead);
    });
}

void ServerWebsocktSession::Send(std::shared_ptr<std::string const> const& message)
{
    // Post our work to the strand, this ensures
    // that the members of `this` will not be
    // accessed concurrently.
    this->mOutQueue.push_back(message);

    if(this->mOutQueue.size() > 1) {
        return;
    }

    //Post to the strand(this->mWebsocketStream.get_executor()) ?
    boost::asio::post(this->mWebsocketStream.get_executor(), [sessionPtr = shared_from_this(), message](){
        sessionPtr->OnSend(message);
    });
}

void ServerWebsocktSession::OnSend(std::shared_ptr<std::string const> const& message)
{
    // We are not currently writing, so send this immediately
    this->mWebsocketStream.async_write(boost::asio::buffer(*this->mOutQueue.front()), [sessionPtr = shared_from_this()](boost::beast::error_code errorCode, std::size_t sentSize){
        sessionPtr->OnWrite(errorCode, sentSize);
    });
}

void ServerWebsocktSession::OnWrite(boost::beast::error_code errorCode, std::size_t sentSize) {
    if(errorCode) {
        this->Fail(errorCode, "Writing");
        return;
    }

    this->mOutQueue.erase(this->mOutQueue.begin());

    if(! this->mOutQueue.empty()) {
        this->mWebsocketStream.async_write(boost::asio::buffer(*this->mOutQueue.front()), [sessionPtr = shared_from_this()](boost::beast::error_code errorCode, std::size_t sentSize){
            sessionPtr->OnWrite(errorCode, sentSize);
        });
    }
}

void ServerWebsocktSession::Fail(boost::beast::error_code errorCode, char const* when) {
    if(errorCode != boost::asio::error::operation_aborted && errorCode != websocket::error::closed) {
        std::cerr << "[Websocket Session] Error during: " << when << "  . Error message is: " << errorCode.message() << "\n";
    }
}
