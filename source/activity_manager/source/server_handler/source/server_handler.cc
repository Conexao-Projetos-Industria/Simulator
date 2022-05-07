#include "server_handler.h"
#include "server_websocket_session.h"

ServerHandler::ServerHandler() {
}

void ServerHandler::HandleHttpRequest(http::request<http::string_body>&& req , std::function<void(http::response<http::string_body>)>&& sendCallback) {
    std::stringstream ss;
    ss << mDebugInt++ << " Unknown HTTP-method";

    http::response<http::string_body> res{http::status::bad_request, req.version()};
    res.set(http::field::server, "[ServerHandler]");
    res.set(http::field::content_type, "text/html");
    res.keep_alive(req.keep_alive());
    res.body() = ss.str();
    res.prepare_payload();

    sendCallback(res);
}

void ServerHandler::HandleWebsocketMessage(ServerWebsocktSession * session, std::string const& message) {
    std::cout << "Recieve websocket message from " << session << std::endl;
    std::cout << "Message is " << message << std::endl;
    this->Broadcast(message);
}

void ServerHandler::Add(ServerWebsocktSession * session) {
    std::lock_guard<std::mutex> lock(this->mSessionsMutex);
    std::cout << "Adding Websocket session " << session << std::endl;
    this->mAllSessions.insert(session);
}

void ServerHandler::Leave(ServerWebsocktSession * session) {
    {
        std::lock_guard<std::mutex> lock(this->mSessionsMutex);
        std::cout << "Removing Websocket session " << session << std::endl;
        auto position = this->mAllSessions.find(session);
        if(position != this->mAllSessions.end()) {
            this->mAllSessions.erase(position);
        }
    }
    this->Broadcast("Colegue leaved");
}

void ServerHandler::Broadcast(std::string message) {
    std::cout << "Broadcasting " << message << std::endl;

    auto const ss = std::make_shared<std::string const>(std::move(message));

    std::vector<std::weak_ptr<ServerWebsocktSession>> sessionsWeakPtrs;
    {
        std::lock_guard<std::mutex> lock(this->mSessionsMutex);

        sessionsWeakPtrs.reserve(this->mAllSessions.size());
        for(auto serverWebsocketSession : this->mAllSessions) {
            sessionsWeakPtrs.emplace_back(serverWebsocketSession->weak_from_this());
        }
    }

    for(auto const& sessionWkPtr : sessionsWeakPtrs) {
        if(auto sessionsPtr = sessionWkPtr.lock()) {
            sessionsPtr->Send(ss);
        }
    }
}
