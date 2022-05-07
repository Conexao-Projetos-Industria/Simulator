#pragma once

#include "iactivity_manager_connector.h"

class ClientWebsocketSession;
class ActivityManagerConnector :public IActivityManagerConnector{
        std::string mTopic;
        std::string mToken;

        std::shared_ptr<ClientWebsocketSession> mWebsocket;

        void AquireToken(std::string const& activityManagerURL);
    public:
        ActivityManagerConnector(std::string const& activityManagerURL,
                                 std::string const& manifestPath,
                                 std::function<void(const std::string&, const std::string&)> parserCallback,
                                 std::string const& messageOnConnect = "");
        virtual void Publish(std::string const& content, std::string const& key);
};
