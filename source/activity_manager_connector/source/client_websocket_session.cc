#include "client_websocket_session.h"

#include <boost/asio/strand.hpp>

#include <iostream>
#include <string>

//TODO(giuliano) Fix this protocol when not tired
#define SEPARATORSTRING "(&**&)"
#define SEPARATORSTRINGSIZE 6
#define CONNECTTOPIC "onConnect(&**&)"

ClientWebsocketSession::ClientWebsocketSession(boost::asio::io_context& ioContext
                                               ,std::function<void(std::string const&, std::string const&)> parserCallback) 
    :mResolver(boost::asio::make_strand(ioContext))
    ,mWebsocketStream(boost::asio::make_strand(ioContext))
    ,mParserCallback(parserCallback)
{}

ClientWebsocketSession::~ClientWebsocketSession(){
    std::cout << "close" << std::endl;
}

void ClientWebsocketSession::Run(std::string const& url, std::string const& queryText) {
    this->mMessage = CONNECTTOPIC + queryText;

    //Leave this here until this method is trusted
    std::cout << "\nAddress " << url << std::endl;
    
    if(url.length() > 6) {
        auto columnPos = url.rfind(":");
        auto foundAt = url.length() - columnPos;
        if(foundAt < 6 && foundAt != 0) {
            //Leave this heve until this method is trusted
            std::cout << " host: " << url.substr(0, columnPos) << "     port: " << url.substr(columnPos+1, url.length() - columnPos) << std::endl;

            boost::asio::ip::tcp::resolver::query query(url.substr(0, columnPos), url.substr(columnPos+1, url.length() - columnPos));
            // Look up the domain name
            mResolver.async_resolve(query, [sessionPtr = shared_from_this()](boost::beast::error_code ec, boost::asio::ip::tcp::resolver::results_type results){
                sessionPtr->OnResolve(ec, results);
            });

            return;
        }
    }

    boost::asio::ip::tcp::resolver::query query(url, "http");

    mResolver.async_resolve(query, [sessionPtr = shared_from_this()](boost::beast::error_code ec, boost::asio::ip::tcp::resolver::results_type results){
        sessionPtr->OnResolve(ec, results);
    });
}

void ClientWebsocketSession::Send(std::string const& message, std::string const& key) {
    std::cout << "[ClientWebsocketSession]Send" << std::endl;
    //TODO(giuliano)Mutex queue out messages
    this->mMessage = key + SEPARATORSTRING + message;
    this->mWebsocketStream.async_write(boost::asio::buffer(this->mMessage), [sessionPtr = shared_from_this()](boost::beast::error_code errorCode, std::size_t bytesTransferred) {
        sessionPtr->OnWrite(errorCode, bytesTransferred);
    });
}

void ClientWebsocketSession::OnResolve(boost::beast::error_code errorCode, boost::asio::ip::tcp::resolver::results_type results) {
    if(errorCode) {
        this->Fail(errorCode, "Resolving");
        return;
    }

    mHost = results.begin()->host_name();

    boost::beast::tcp_stream * socket = &boost::beast::get_lowest_layer(this->mWebsocketStream);

    // Set timeout
    socket->expires_after(std::chrono::seconds(30));

    socket->async_connect(results, [sessionPtr = shared_from_this()](boost::beast::error_code errorCode, boost::asio::ip::tcp::resolver::results_type::endpoint_type endpointType){
        sessionPtr->OnConnect(errorCode, endpointType);
    });
}

void ClientWebsocketSession::OnConnect(boost::beast::error_code errorCode, boost::asio::ip::tcp::resolver::results_type::endpoint_type endpoint) {
    if(errorCode) {
        this->Fail(errorCode, "Connecting.");
        return;
    }

    boost::beast::tcp_stream * socket = &boost::beast::get_lowest_layer(this->mWebsocketStream);
    
    //Set timeout
    socket->expires_never();
    this->mWebsocketStream.set_option(boost::beast::websocket::stream_base::timeout {std::chrono::seconds(30) , std::chrono::steady_clock::duration::max() , false});
    
    // User-Agent of the handshake
    this->mWebsocketStream.set_option(boost::beast::websocket::stream_base::decorator([sessionPtr = shared_from_this()](boost::beast::websocket::request_type& req) {
            req.set(boost::beast::http::field::user_agent, "Activity Manager Connector");
    }));

    this->mHost = endpoint.address().to_string() + ':' + std::to_string(endpoint.port());

    // Promote to websocket
    this->mWebsocketStream.async_handshake(this->mHost, "/", [sessionPtr = shared_from_this()](boost::beast::error_code errorCode) {
        if(errorCode) {
            sessionPtr->Fail(errorCode, "Handshake.");
            return;
        }

        sessionPtr->mWebsocketStream.async_write(boost::asio::buffer(sessionPtr->mMessage), [sessionPtr](boost::beast::error_code errorCode, std::size_t bytesTransferred) {
            // sessionPtr->OnWrite(errorCode, bytesTransferred);
            boost::ignore_unused(bytesTransferred);

            if(errorCode) {
                sessionPtr->Fail(errorCode, "Write.");
                return;
            }

            // Read a message into our buffer
            sessionPtr->mWebsocketStream.async_read(sessionPtr->mBuffer,[sessionPtr](boost::beast::error_code errorCode, std::size_t bytesTransferred) {
                sessionPtr->OnRead(errorCode, bytesTransferred);
            });
        });
    });
}

void ClientWebsocketSession::OnWrite(boost::beast::error_code errorCode, std::size_t bytesTransferred) {
    boost::ignore_unused(bytesTransferred);

    if(errorCode) {
        this->Fail(errorCode, "Write.");
        return;
    }

    // Read a message into our buffer
    // this->mWebsocketStream.async_read(this->mBuffer,[sessionPtr = shared_from_this()](boost::beast::error_code errorCode, std::size_t bytesTransferred) {
    //     sessionPtr->OnRead(errorCode, bytesTransferred);
    // });
}

void ClientWebsocketSession::OnRead(boost::beast::error_code errorCode, std::size_t bytesTransferred) {
    // boost::ignore_unused(bytesTransferred);

    if(errorCode) {
        this->Fail(errorCode, "Read.");
        return;
    }

    //TODO(giuliano) Better this protocol, please..
    auto message = boost::beast::buffers_to_string(this->mBuffer.data());

    auto space = message.find(SEPARATORSTRING);
    this->mParserCallback(message.substr(space+SEPARATORSTRINGSIZE, std::string::npos), message.substr(0, space));

    this->mBuffer.consume(this->mBuffer.size());

    // Read a message into our buffer
    this->mWebsocketStream.async_read(this->mBuffer,[sessionPtr = shared_from_this()](boost::beast::error_code errorCode, std::size_t bytesTransferred) {
        sessionPtr->OnRead(errorCode, bytesTransferred);
    });
}

void ClientWebsocketSession::Close() {
    this->mWebsocketStream.async_close(boost::beast::websocket::close_code::normal,[sessionPtr = shared_from_this()](boost::beast::error_code errorCode) {
        sessionPtr->OnClose(errorCode);
    });
}

void ClientWebsocketSession::OnClose(boost::beast::error_code errorCode) {
    if(errorCode) {
        this->Fail(errorCode, "Close.");
        return;
    }
}

void ClientWebsocketSession::Fail(boost::beast::error_code errorCode, char const* when) {
    if(errorCode != boost::asio::error::operation_aborted) {
        std::cerr << "[Client Http Session] Error during: " << when << "  . Error message is: " << errorCode.message() << "\n";
    }
}

// std::stringstream ss;
// ss << "mymessage";

// boost::beast::http::response<boost::beast::http::string_body> res{boost::beast::http::status::bad_request, 11};
// res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
// res.set(boost::beast::http::field::content_type, "text/html");
// res.keep_alive(true);
// res.body() = ss.str();
// res.prepare_payload();

// // sendCallback(res);
// auto callback = [sessionPtr] (<boost::beast::http::response<<boost::beast::http::string_body>&& response) {
//     // Garantee that the life of the message get extended
//     auto responsePtr = std::make_shared<boost::beast::http::response<boost::beast::http::string_body>>(response);

//     boost::beast::http::async_write(sessionPtr->mSocket, *responsePtr, [sessionPtr, responsePtr] (boost::beast::error_code errorCode, std::size_t bytes) {
//         sessionPtr->OnWrite(errorCode, bytes, responsePtr->need_eof());
//     });
// };
