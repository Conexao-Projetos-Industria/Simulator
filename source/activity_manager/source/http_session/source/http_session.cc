#include "http_session.h"
#include "websocket_session.h"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/websocket.hpp>

#include <iostream>

namespace beast = boost::beast;
using tcp = boost::asio::ip::tcp;
namespace http = boost::beast::http;
namespace websocket = boost::beast::websocket;

HttpSession::HttpSession(tcp::socket socket, const std::shared_ptr<ServerHandler>& state)
    : mSocket(std::move(socket))
    , mState(state)
{}

void HttpSession::Run() {
    http::async_read(mSocket, mBuffer, mRequest, [httpSessionPtr = shared_from_this()] (beast::error_code errorCode, std::size_t bytesRead)
    {
        httpSessionPtr->OnRead(errorCode, bytesRead);
    });
}

void HttpSession::OnRead(beast::error_code errorCode, std::size_t bytesRead) {
    if(errorCode == http::error::end_of_stream) {
        this->mSocket.shutdown(tcp::socket::shutdown_send, errorCode);
        return;
    }

    if(errorCode) {
        this->Fail(errorCode, "Reading");
        return;
    }

    if(websocket::is_upgrade(this->mRequest)) {
        auto websocketSession = std::make_shared<WebsocktSession>(std::move(this->mSocket), this->mState);
        websocketSession->Start(std::move(this->mRequest));
        return;
    }

    this->mState->HandleHttpRequest(std::move(this->mRequest), [sessionPtr = shared_from_this()] (auto&& response) {
        using responseType = typename std::decay<decltype(response)>::type;
        auto responsePtr = std::make_shared<responseType>(std::forward<decltype(response)>(response));

        http::async_write(sessionPtr->mSocket, *responsePtr, [sessionPtr, responsePtr] (beast::error_code errorCode, std::size_t bytes) {
            sessionPtr->OnWrite(errorCode, bytes, responsePtr->need_eof());
        });

    });
}

void HttpSession::OnWrite(beast::error_code errorCode, std::size_t, bool close) {
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

void HttpSession::Fail(beast::error_code errorCode, char const* when) {
    if(errorCode != boost::asio::error::operation_aborted) {
        std::cerr << "[Http Session] Error during: " << when << "  . Error message is: " << errorCode.message() << "\n";
    }
}
