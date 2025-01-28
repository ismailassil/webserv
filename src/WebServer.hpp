#pragma once

#include <string>
// kqueue
#include "ConfigParser.hpp"

class WebServer {
private:
    std::vector<Server> servers;
    ConfigParser configParser;
    // void setupKqueue();                         // Initialize kqueue
    // void registerSocket(int sockfd);            // Register a socket with kqueue
    // void handleEvent(struct kevent& event);     // Handle a kqueue event
    // void acceptConnection(int listenSock);      // Accept new connections
    // void handleClientSocket(int clientSock);    // Handle data from a client socket
public:
    WebServer();
    void    initialize(const std::string& configFile);
    void    run();
    void    shutdown();
    void    displayData();
};
