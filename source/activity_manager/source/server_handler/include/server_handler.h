#pragma once

#include <boost/beast/websocket.hpp>
#include <string>
#include <unordered_set>
#include <mutex>

namespace http = boost::beast::http;

class WebsocktSession;
class ServerHandler {
    private:
        std::mutex mSessionsMutex;
        std::unordered_set<WebsocktSession*> mSessions;

    public:
        ServerHandler();
        template<class Body, class Allocator, class Send>
        void HandleHttpRequest (http::request<Body, http::basic_fields<Allocator>>&& req, Send&& send) {};
        void HandleWebsocketMessage (WebsocktSession* session, std::string message) {};
        void Add (WebsocktSession* session) {};
        void Leave (WebsocktSession* session) {};
        void Broadcast (std::string message) {};
};
