/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 15:36:29 by iassil            #+#    #+#             */
/*   Updated: 2024/12/21 04:40:13 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../bits/RequestBuilder.hpp"
#include <cstddef>
#include <exception>
#include <iostream>
#include <cstring>
#include <fstream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 8192

int main() {
	srand(time(NULL));

	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0) {
		std::cout << "socket failed" << std::endl;
		exit(1);
	}

	int yes = 1;
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

	struct sockaddr_in server_addr;
	socklen_t server_addr_len = sizeof(server_addr);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(PORT);

	if (bind(server_fd, (struct sockaddr*)&server_addr, server_addr_len) == -1) {
		std::cout << "bind failed" << std::endl;
		exit(1);
	}

	if (listen(server_fd, 1000) == -1) {
		std::cout << "listen failed" << std::endl;
		exit(1);
	}

	RequestBuilder req;
	std::cout << "Server is listening on localhost port: " << PORT << std::endl;

	while (1) {
		struct sockaddr_in client_address;
		socklen_t addrlen = sizeof(client_address);
		int new_socket = accept(server_fd, (struct sockaddr*)&client_address, &addrlen);
		if (new_socket < 0) {
			std::cout << "accept failed" << std::endl;
			exit(1);
		}

		std::cout << "=================START=================" << std::endl;

		char buffer[BUFFER_SIZE] = {0};
		int bytes_received;
		int step = 0;

		while ((bytes_received = recv(new_socket, buffer, sizeof(buffer), 0)) > 0) {
			std::string buf(buffer, bytes_received);

			std::ofstream ss("request.py", std::ios::app | std::ios::binary);
			std::ofstream sss("chunks.py", std::ios::app | std::ios::binary);
			ss.write(buf.c_str(), buf.length());

			sss << "\n===========================================================\n";
			sss.write(buf.c_str(), buf.length());
			// double start = clock();

			try {

				req.build(buf);

				// Output the elapsed time
				// std::cout << "Time for req.build(buf): " << double(clock() - start) / CLOCKS_PER_SEC * 1000.0 << "ms" << endl;
			} catch (const char* e) {
				std::cerr << "Error: " << e << std::endl;
			} catch (const std::exception& ee) {
				std::cerr << "Error: " << ee.what() << std::endl;
			}
			std::cout << "=======" << step++ << "=======" << std::endl;
		}
		std::cout << "=================END=================" << std::endl;

		if (bytes_received < 0) {
			std::cout << "recv failed or connection closed\n";
			close(new_socket);
			continue;
		}
	}
	close(server_fd);
	return 0;
}
