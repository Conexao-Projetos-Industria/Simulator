#include "activity_manager_connector.h"
#include "client_http_session.h"
#include "client_websocket_session.h"

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/strand.hpp>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <sstream>

ActivityManagerConnector::ActivityManagerConnector(const std::string& activityManagerURL,
                                                   const std::string& manifestPath,
                                                   std::function<void(const std::string&, const std::string&)> parserCallback,
                                                   std::string const& messageOnConnect) 
{
    this->AquireToken(activityManagerURL);

    auto contextPtr = std::make_shared<boost::asio::io_context>();

    this->mWebsocket = std::make_shared<ClientWebsocketSession>(*contextPtr, parserCallback);

    this->mWebsocket->Run(activityManagerURL, messageOnConnect);

    contextPtr->run();
}

//Connect with activivty manager and get a token. (In the future it will be used for security)
void ActivityManagerConnector::AquireToken(std::string const& activityManagerURL) {
    std::cout << "[ActivityManagerConnector] Read on connection: " << ClientHttpSession::Get("192.168.0.17:9091", "[ActivityManagerConnector]Token");
}

void ActivityManagerConnector::Publish(std::string const& content, std::string const& key) {
    std::cout << "[ActivityManagerConnector]Publish" << std::endl;
    mWebsocket->Send(content, key);
}
