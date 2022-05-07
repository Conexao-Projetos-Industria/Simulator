#include "client_http_session.h"

#include <boost/asio/strand.hpp>
#include <boost/asio/buffer.hpp>

#include <iostream>
#include <string>

std::string ClientHttpSession::Get(std::string const& url, std::string const&& queryText) {
    std::string httpResponse;

    boost::asio::io_context ioContext;
    auto httpSession = std::make_shared<ClientHttpSession>(ioContext, [&httpResponse](std::string const& returnedString){
        httpResponse = returnedString;
    });

    httpSession->GetUrl("192.168.0.17:9091", std::move(queryText));

    ioContext.run();

    return httpResponse;
}

ClientHttpSession::ClientHttpSession(boost::asio::io_context& ioContext
                                     ,std::function<void(std::string const&)> parserCallback) 
    :mResolver(boost::asio::make_strand(ioContext))
    ,mTcpStream(boost::asio::make_strand(ioContext))
    ,mParserCallback(parserCallback)
{}

void ClientHttpSession::GetUrl(std::string const& url, std::string const&& queryText){
    this->mMessage = queryText;

    auto parsedUrl = this->Parse(url);

    this->mRequest.version(11);
    this->mRequest.method(boost::beast::http::verb::get);
    this->mRequest.target(parsedUrl.target);
    this->mRequest.set(boost::beast::http::field::host, parsedUrl.ip + ':' + parsedUrl.port);
    this->mRequest.set(boost::beast::http::field::user_agent, "ClientHttpSession");
    this->mRequest.body() = queryText;

    boost::asio::ip::tcp::resolver::query query(parsedUrl.ip, parsedUrl.port);

    mResolver.async_resolve(query, [sessionPtr = shared_from_this()](boost::beast::error_code ec, boost::asio::ip::tcp::resolver::results_type results){
        sessionPtr->OnResolve(ec, results);
    });
}

void ClientHttpSession::OnResolve(boost::beast::error_code errorCode, boost::asio::ip::tcp::resolver::results_type results) {
    if(errorCode) {
        this->Fail(errorCode, "Resolving");
        return;
    }

    this->mTcpStream.expires_after(std::chrono::seconds(30));

    this->mTcpStream.async_connect(results, [sessionPtr = shared_from_this()](boost::beast::error_code errorCode, boost::asio::ip::tcp::resolver::results_type::endpoint_type endpointType){
        sessionPtr->OnConnect(errorCode, endpointType);
    });
}

void ClientHttpSession::OnConnect(boost::beast::error_code errorCode, boost::asio::ip::tcp::resolver::results_type::endpoint_type endpoint) {
    if(errorCode) {
        this->Fail(errorCode, "Connecting.");
        return;
    }

    boost::beast::http::request<boost::beast::http::string_body> req{boost::beast::http::verb::get, "/", 11};
    req.set(boost::beast::http::field::server, "ClientHttpSession");
    req.set(boost::beast::http::field::content_type, "text/html");
    req.keep_alive(false);
    req.body() = this->mMessage;
    req.prepare_payload();

    [sessionPtr = shared_from_this()] (boost::beast::http::request<boost::beast::http::string_body>& request) {
        // Garantee that the life of the message get extended
        auto requestPtr = std::make_shared<boost::beast::http::request<boost::beast::http::string_body>>(request);

        boost::beast::http::async_write(sessionPtr->mTcpStream, *requestPtr, [sessionPtr, requestPtr] (boost::beast::error_code errorCode, std::size_t bytes) {
            sessionPtr->OnWrite(errorCode, bytes);
        });
    }(req);
    // callback(res);

    // boost::beast::http::async_write(this->mTcpStream, this->mRequest, [sessionPtr = shared_from_this()](boost::beast::error_code errorCode, std::size_t bytesTransferred){
    //     sessionPtr->OnWrite(errorCode, bytesTransferred);
    // });
}

void ClientHttpSession::OnWrite(boost::beast::error_code errorCode, std::size_t bytesTransferred) {
    boost::ignore_unused(bytesTransferred);

    if(errorCode) {
        this->Fail(errorCode, "Write.");
        return;
    }

    boost::beast::http::async_read(this->mTcpStream, this->mBuffer, this->mResponse, [sessionPtr = shared_from_this()](boost::beast::error_code errorCode, std::size_t bytesTransferred) {
        sessionPtr->OnRead(errorCode, bytesTransferred);
    });
}

void ClientHttpSession::OnRead(boost::beast::error_code errorCode, std::size_t bytesTransferred) {
    if(errorCode) {
        this->Fail(errorCode, "Read.");
        return;
    }

    this->mParserCallback(this->mResponse.body());

    boost::ignore_unused(bytesTransferred);
    this->mTcpStream.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, errorCode);

    if(errorCode && errorCode != boost::beast::errc::not_connected) {
        this->Fail(errorCode, "Shutdown");
    }
}

ParsedUrl ClientHttpSession::Parse(std::string const& url) {
    //TODO(giuliano) Find a better way.

    if(url.length() > 6) {
        auto columnPos = url.rfind(":");
        auto foundAt = url.length() - columnPos;
        if(foundAt < 6 && foundAt != 0) {
            return{url.substr(0, columnPos), url.substr(columnPos+1, url.length() - columnPos), "/"};
        }
    }

    return{url, "", "/"};
}

void ClientHttpSession::Fail(boost::beast::error_code errorCode, char const* when) {
    if(errorCode != boost::asio::error::operation_aborted) {
        std::cerr << "[Client Http Session] Error during: " << when << "  . Error message is: " << errorCode.message() << "\n";
    }
}
