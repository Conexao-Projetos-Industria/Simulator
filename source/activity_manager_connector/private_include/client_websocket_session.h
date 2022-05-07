#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>

#include <memory>

class ClientWebsocketSession : public std::enable_shared_from_this<ClientWebsocketSession> {
    boost::asio::ip::tcp::resolver mResolver;
    boost::beast::websocket::stream<boost::beast::tcp_stream> mWebsocketStream;
    std::function<void(const std::string&, const std::string&)> mParserCallback;

    boost::beast::flat_buffer mBuffer;
    std::string mMessage;
    std::string mHost;

    void OnResolve(boost::beast::error_code errorCode, boost::asio::ip::tcp::resolver::results_type results);
    void OnConnect(boost::beast::error_code errorCode, boost::asio::ip::tcp::resolver::results_type::endpoint_type endpoint);
    void Fail(boost::beast::error_code errorCode, char const* when);
    void OnWrite(boost::beast::error_code errorCode, std::size_t bytesTransferred);
    void OnRead(boost::beast::error_code errorCode, std::size_t bytesTransferred);
    void OnClose(boost::beast::error_code errorCode);
    public:
        explicit ClientWebsocketSession(boost::asio::io_context& ioContext
                                        ,std::function<void(std::string const&, std::string const&)> parserCallback);
        ClientWebsocketSession::~ClientWebsocketSession();
        void Run(std::string const& url, std::string const& query = "");
        void Send(std::string const& message, std::string const& key);
        void Close();
};
