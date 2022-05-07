#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>

#include <memory>

typedef struct{std::string ip; std::string port; std::string target;} ParsedUrl;

class ClientHttpSession : public std::enable_shared_from_this<ClientHttpSession> {
    boost::asio::ip::tcp::resolver mResolver;
    boost::beast::tcp_stream mTcpStream;
    std::function<void(const std::string&)> mParserCallback;

    boost::beast::flat_buffer mBuffer;
    std::string mMessage;
    std::string mHost;

    boost::beast::http::request<boost::beast::http::string_body> mRequest;
    boost::beast::http::response<boost::beast::http::string_body> mResponse;

    void OnResolve(boost::beast::error_code errorCode, boost::asio::ip::tcp::resolver::results_type results);
    void OnConnect(boost::beast::error_code errorCode, boost::asio::ip::tcp::resolver::results_type::endpoint_type endpoint);
    void OnWrite(boost::beast::error_code errorCode, std::size_t bytesTransferred);
    void OnRead(boost::beast::error_code errorCode, std::size_t bytesTransferred);
    void Fail(boost::beast::error_code errorCode, char const* when);
    
    ParsedUrl Parse(std::string const& url);
    public:
        explicit ClientHttpSession(boost::asio::io_context& ioContext
                                   ,std::function<void(std::string const&)> parserCallback);
        void GetUrl(std::string const& url, std::string const&& queryText = "");
        static std::string Get(std::string const& url, std::string const&& queryText = "");
};
