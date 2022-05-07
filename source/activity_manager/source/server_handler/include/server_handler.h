#pragma once

#include <boost/beast/websocket.hpp>
#include <string>
#include <unordered_set>
#include <mutex>
#include <iostream>

namespace http = boost::beast::http;

class ServerWebsocktSession;
class ServerHandler {
    private:
        std::mutex mSessionsMutex;
        std::unordered_set<ServerWebsocktSession*> mAllSessions;
        std::unordered_multimap<std::string, ServerWebsocktSession*> mTopicsMap;
        int mDebugInt = 0;

    public:
        ServerHandler();

        void HandleHttpRequest (http::request<http::string_body>&& req , std::function<void(http::response<http::string_body>)>&& sendCallback);
        void HandleWebsocketMessage (ServerWebsocktSession* session, std::string const& message);
        void Add (ServerWebsocktSession* session);
        void Leave (ServerWebsocktSession* session);
        void Broadcast (std::string message);
};
