#pragma once
#include "../config/ConfigParser.hpp"
#include "bits.hpp"	 // IWYU pragma: keep

class WebServer {
	private:
		vector< Server > servers;
		ConfigParser	 configParser;
		// void setupKqueue();                         // Initialize kqueue
		// void registerSocket(int sockfd);            // Register a socket with kqueue
		// void handleEvent(struct kevent& event);     // Handle a kqueue event
		// void acceptConnection(int listenSock);      // Accept new connections
		// void handleClientSocket(int clientSock);    // Handle data from a client socket

	public:
		WebServer();

		void init( const string& config_file );
		void run();
		void shutdown();
		void displayData();
};
