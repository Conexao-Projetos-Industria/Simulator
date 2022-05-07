#include "server_http_session.h"
#include "server_websocket_session.h"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/websocket.hpp>

#include <iostream>

namespace beast = boost::beast;
using tcp = boost::asio::ip::tcp;
namespace http = boost::beast::http;
namespace websocket = boost::beast::websocket;

ServerHttpSession::ServerHttpSession(tcp::socket socket, const std::shared_ptr<ServerHandler>& state)
    : mSocket(std::move(socket))
    , mState(state)
{}

void ServerHttpSession::Run() {
    http::async_read(mSocket, mBuffer, mRequest, [httpSessionPtr = shared_from_this()] (beast::error_code errorCode, std::size_t bytesRead)
    {
        httpSessionPtr->OnRead(errorCode, bytesRead);
    });
}

void ServerHttpSession::OnRead(beast::error_code errorCode, std::size_t bytesRead) {
    if(errorCode == http::error::end_of_stream) {
        this->mSocket.shutdown(tcp::socket::shutdown_send, errorCode);
        return;
    }

    if(errorCode) {
        this->Fail(errorCode, "Reading");
        return;
    }

    if(websocket::is_upgrade(this->mRequest)) {
        auto websocketSession = std::make_shared<ServerWebsocktSession>(std::move(this->mSocket), this->mState);
        websocketSession->Start(std::move(this->mRequest));
        return;
    }

    std::cout << "\n[ServerHttpSession] Reading " << bytesRead << std::endl;
    std::cout << "Method " << this->mRequest.method() << std::endl;
    for(auto const& it : this->mRequest)
    {
        std::cout << it.name() << " = " << it.value() << std::endl;
    }
    std::cout << this->mRequest.body() << std::endl;

    this->mState->HandleHttpRequest(std::move(this->mRequest), [sessionPtr = shared_from_this()] (http::response<http::string_body>&& response) {
        // Garantee that the life of the message get extended
        auto responsePtr = std::make_shared<http::response<http::string_body>>(response);

        http::async_write(sessionPtr->mSocket, *responsePtr, [sessionPtr, responsePtr] (beast::error_code errorCode, std::size_t bytes) {
            sessionPtr->OnWrite(errorCode, bytes, responsePtr->need_eof());
        });
    });
}

void ServerHttpSession::OnWrite(beast::error_code errorCode, std::size_t, bool close) {
    if(errorCode) {
        this->Fail(errorCode, "Reading");
        return;
    }

    if(close) {
        this->mSocket.shutdown(tcp::socket::shutdown_send, errorCode);
        return;
    }

    this->mRequest = {};

    this->Run();
}

void ServerHttpSession::Fail(beast::error_code errorCode, char const* when) {
    if(errorCode != boost::asio::error::operation_aborted) {
        std::cerr << "[Http Session] Error during: " << when << "  . Error message is: " << errorCode.message() << "\n";
    }
}
