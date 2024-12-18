/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 15:36:29 by iassil            #+#    #+#             */
/*   Updated: 2024/12/18 11:51:52 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../bits/RequestBuilder.hpp"
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fstream>
#include <sstream>

#include <fstream>
#include <ostream>
#include <cstring>
#include <vector>


#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/event.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <exception>
#include <iostream>
#include <sys/event.h>
#include <sys/time.h>
#include <unistd.h>

class Server{

public:
	std::string host;
	double port;
	Server();
	Server(std::string host, int port);
	int		ft_server_init();
	static void	ft_start(int size, int *fd);
	std::string buffering_header(std::string msg, int first_time);
	~Server();
};


#define MAX_CLIENTS 128
#define BUFFER_SIZE 4096


void Server::ft_start(int size, int *fd) {


	RequestBuilder request;
	int kq = kqueue();
	if (kq == -1) {
		std::cerr << "kqueue failed" << std::endl;
		exit(1);
	}

	// Set up events for each server listening socket
	struct kevent event;
	for (int i = 0; i < size; ++i) {
		EV_SET(&event, fd[i], EVFILT_READ, EV_ADD, 0, 0, NULL);
		if (kevent(kq, &event, 1, NULL, 0, NULL) == -1) {
			std::cerr << "kevent failed for server " << i << std::endl;
			exit(1);
		}
	}

	// Array to store client socket file descriptors
	int client_sockets[MAX_CLIENTS] = {0};
	std::string client_buffers[MAX_CLIENTS]; // Store data to be sent

	while (true) {
		struct kevent events[MAX_CLIENTS];
		int n = kevent(kq, NULL, 0, events, MAX_CLIENTS, NULL); // Wait for events
		if (n < 0) {
			std::cerr << "kevent failed" << std::endl;
			exit(1);
		}

		for (int i = 0; i < n; ++i) {
			bool is_new_connection = false;

			// Check if the event corresponds to one of the server's listening sockets
			for (int j = 0; j < size; ++j) {
				if ((int)events[i].ident == fd[j] && events[i].filter == EVFILT_READ) {
					// Accept new client connection
					struct sockaddr_in client_addr;
					socklen_t addrlen = sizeof(client_addr);
					int new_socket = accept(fd[j], (struct sockaddr*)&client_addr, &addrlen);
					if (new_socket < 0) {
						std::cerr << "accept failed" << std::endl;
						continue;
					}

					std::cout << "New client connected on server " << j << ": " << new_socket << std::endl;

					// Add the new socket to the kqueue for future reading events
					EV_SET(&event, new_socket, EVFILT_READ, EV_ADD, 0, 0, NULL);
					kevent(kq, &event, 1, NULL, 0, NULL);

					// Optionally, store the socket in an array for further management
					for (int k = 0; k < MAX_CLIENTS; ++k) {
						if (client_sockets[k] == 0) {
							client_sockets[k] = new_socket;
							break;
						}
					}

					is_new_connection = true;
					break;
				}
			}

			// If it's not a new connection, it's a client sending data
			if (!is_new_connection && events[i].filter == EVFILT_READ) {
				int client_socket = events[i].ident;
				char buffer[BUFFER_SIZE] = {0};
				int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
				if (bytes_received <= 0) {
					// Client has closed the connection or error occurred
					std::cout << "Client " << client_socket << " disconnected or error occurred." << std::endl;
					close(client_socket);
					EV_SET(&event, client_socket, EVFILT_READ, EV_DELETE, 0, 0, NULL);
					kevent(kq, &event, 1, NULL, 0, NULL);

					// Remove from the client list
					for (int j = 0; j < MAX_CLIENTS; ++j) {
						if (client_sockets[j] == client_socket) {
							client_sockets[j] = 0;
							break;
						}
					}
				} else {
					// Handle incoming message (echo or any other processing)


					std::string msg;
					msg.assign(buffer, bytes_received);
					ofstream ss("request.py", ios::app);
					ss << "Received from client:"  << msg;
					
					// std::cout << RED <<  "Received from client: "<< RESET  << msg << std::endl;
					// if (msg.find("POST") != std::string::npos) {

					////////////////////////////////////////////////////
					////////////////////////////////////////////////////
					////////////////////////////////////////////////////
					request.build(msg);
					////////////////////////////////////////////////////
					////////////////////////////////////////////////////
					////////////////////////////////////////////////////
					
					




		

					// 	// Prepare a response to send
					// 	std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 11\r\n\r\nHello world";
					// 	client_buffers[client_socket] = response;

					// 	// Add a write event to the kqueue
					// 	EV_SET(&event, client_socket, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
					// 	kevent(kq, &event, 1, NULL, 0, NULL);
					// } else
					//  if (msg.find("GET") != std::string::npos) {
					// 	std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 12\r\n\r\nHello world3";
					// 	send(client_socket, response.c_str(), response.size(), 0);
					// }
				}
			} else if (events[i].filter == EVFILT_WRITE) {
				int client_socket = events[i].ident;
				std::string &response = client_buffers[client_socket];
				int bytes_sent = send(client_socket, response.c_str(), response.length(), 0);
				if (bytes_sent < 0) {
					std::cerr << "send failed" << std::endl;
					close(client_socket);
					EV_SET(&event, client_socket, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
					kevent(kq, &event, 1, NULL, 0, NULL);

					// Remove from the client list
					for (int j = 0; j < MAX_CLIENTS; ++j) {
						if (client_sockets[j] == client_socket) {
							client_sockets[j] = 0;
							break;
						}
					}
				} else {
					// Remove the write event after sending the response
					EV_SET(&event, client_socket, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
					kevent(kq, &event, 1, NULL, 0, NULL);

					// Close the client connection after serving
					close(client_socket);
					EV_SET(&event, client_socket, EVFILT_READ, EV_DELETE, 0, 0, NULL);
					kevent(kq, &event, 1, NULL, 0, NULL);

					// Remove from the client list
					for (int j = 0; j < MAX_CLIENTS; ++j) {
						if (client_sockets[j] == client_socket) {
							client_sockets[j] = 0;
							break;
						}
					}
				}
			}
		}
	}

	// Cleanup: Close all server and client connections
	for (int i = 0; i < size; ++i) {
		close(fd[i]);
	}
}

int Server::ft_server_init() {

    if (this->port < 1024 || this->port > 65535) {
        std::cout << "Error port number\n";
        exit(1);
    }

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cout << "socket failed" << std::endl;
        exit(1);
    }

    // Enable SO_REUSEADDR to allow quick reuse of the port
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "setsockopt(SO_REUSEADDR) failed" << std::endl;
        close(server_fd);
        exit(1);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(this->host.c_str()); // by default Binding to localhost
    server_addr.sin_port = htons(this->port); // by default Port 8080

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cout << "bind failed" << std::endl;
        close(server_fd);
        exit(1);
    }

    std::cout << "Server is listening on " << this->host << ":" << this->port << std::endl;
    if (listen(server_fd, MAX_CLIENTS) == -1) {
        std::cout << "listen failed" << std::endl;
        close(server_fd);
        exit(1);
    }

    return (server_fd);
}

Server::Server(std::string host, int port) {
	this->host = host;
	this->port = port;
}

Server::Server() {
	this->host = "127.0.0.1";
	this->port = 8080;
}
																																									
Server::~Server() {
	std::cout << "good bye...\n";
}


struct dt
{
	string key;
	string val;
};
class File_Parsing
{
	public :
		int	open_b;
		int	close_b;
		ifstream file;
		string conf_path;
		int	servers_count;
		DynamicStruct obj;
		DynamicStruct servers;
		vector<dt> host_port;

		DynamicStruct locations[1000];

		File_Parsing(void);
		File_Parsing(string conf_path);
		~File_Parsing(void);
		DynamicStruct	recursive_push(ifstream *file, string parent, int *open_bracket, int *close_bracket);
		void	Checking_Hierarchy(DynamicStruct &block, DynamicStruct *server, DynamicStruct *locations, const string &name = "");

		void	Struct_Call(DynamicStruct inner);
		void	get_host_name(void);
		location_data	get_location_val(DynamicStruct location);
};
struct location_data
{
	string root;
	string	path;
	string methods;
	string	directory_listing;
	string	cgi;
	string 	rturn;
};


int main(int argc, char **argv)
{
	try{
	///////////////////////////////////////
		if (argc != 2)
			return 0;
		char *end = NULL;

		int size = p.servers_count;
		Server* server[size];
		int fd[size];
		int i = 0;

		// Initialize servers
		for (std::vector<dt>::iterator it = p.host_port.begin(); it != p.host_port.end(); it++)
		{
			server[i++] = new Server(it->key, std::strtod(it->val.c_str(), &end));
			if (*end)
				return 1;
		}
		i = 0;
		while (i < size)
		{
			// Initialize each server and get their listening socket fd
			fd[i] = server[i]->ft_server_init();
			i++;
		}

		Server::ft_start(size, fd);
	}
	catch(const std::exception &e)
	{
	    std::cout << e.what() << std::endl;
	}



	return 0;
}