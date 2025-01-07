#include "ConfigParser.hpp"

int main()
{
    ConfigParser ConfigParser;
    ConfigParser.parseConfigFile("server.conf");
    ConfigParser.displayData();
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1)
    {
        std::cerr << "Socket Creation Error" << std::endl;
        exit(1);
    }
    sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = INADDR_ANY;
    sockaddr.sin_port = htons(9999);
    if (bind(sock_fd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) {
        std::cerr << "Failed to bind to port 9999. errno: " << errno << std::endl;
        exit(1);
    }
    if (listen(sock_fd, 10) < 0) {
        std::cerr << "Failed to listen on socket. errno: " << errno << std::endl;
        exit(1);
    }
    unsigned long addrlen = sizeof(sockaddr);
    int connection = accept(sock_fd, (struct sockaddr*)&sockaddr, (socklen_t*)&addrlen);
    if (connection < 0) {
        std::cout << "Failed to grab connection. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    char buffer[100];
    read(connection, buffer, 100);
    std::cout << "The message was: " << buffer;

    std::string response = "Good talking to you\n";
    send(connection, response.c_str(), response.size(), 0);

    close(connection);
    close(sock_fd);
}
