#include "server_handler.h"
#include "server.h"

#include <boost/beast/core.hpp>

#include <sstream>
#include <iostream>

int main(int argc, char* argv[])
{
    boost::asio::ip::address address;
    unsigned short port;

    if (argc != 3) {
        std::cout << "You can pass server ip and port as arguments.\n Example:\n .\\activity_manager.exe \"192.168.0.17\" \"9091\"";
        std::cout << "\nWhat is the address the Activity Manager is hosted on? (ex.: 0.0.0.0)\n";
        std::string userAddress;
        std::cin >> userAddress;
        address = boost::asio::ip::make_address(userAddress.c_str());

        std::cout << "What is should be the Activity Manager port? (ex.: 8080)\n";
        std::cin >> port;
    } else {
        address = boost::asio::ip::make_address(argv[1]);
        port = static_cast<unsigned short>(std::atoi(argv[2]));
    }

    boost::asio::io_context ioContext;

    auto server = std::make_shared<Server>(ioContext, boost::asio::ip::tcp::endpoint{address, port}, std::make_shared<ServerHandler>());
    server->Run();

    ioContext.run();
}
