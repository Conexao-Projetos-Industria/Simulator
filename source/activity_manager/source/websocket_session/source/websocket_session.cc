#include <websocket_session.h>

#include <iostream>

WebsocktSession::WebsocktSession(tcp::socket socket, const std::shared_ptr<ServerHandler>& state)
    : mWebsocketStream(std::move(socket))
    , mState(state)
{}

WebsocktSession::~WebsocktSession()
{
    this->mState->Leave(this);
}

void WebsocktSession::OnAccept(boost::beast::error_code errorCode) {
    if(errorCode) {
        this->Fail(errorCode, "Accepting");
        return;
    }

    this->mState->Add(this);

    this->mWebsocketStream.async_read(this->mBuffer, [sessionPtr = shared_from_this()](boost::beast::error_code errorCode, std::size_t bytesRead) {
        sessionPtr->OnRead(errorCode, bytesRead);
    });
}

void WebsocktSession::OnRead(boost::beast::error_code errorCode, std::size_t bytesRead) {
    if(errorCode) {
        this->Fail(errorCode, "Reading");
        return;
    }

    this->mState->HandleWebsocketMessage(this, boost::beast::buffers_to_string(this->mBuffer.data()));

    this->mBuffer.consume(this->mBuffer.size());

    this->mWebsocketStream.async_read(this->mBuffer, [sessionPtr = shared_from_this()](boost::beast::error_code errorCode, std::size_t bytesRead) {
        sessionPtr->OnRead(errorCode, bytesRead);
    });
}

void WebsocktSession::Fail(boost::beast::error_code errorCode, char const* when) {
    if(errorCode != boost::asio::error::operation_aborted && errorCode != websocket::error::closed) {
        std::cerr << "[Websocket Session] Error during: " << when << "  . Error message is: " << errorCode.message() << "\n";
    }
}
