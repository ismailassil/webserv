/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 15:36:29 by iassil            #+#    #+#             */
/*   Updated: 2024/12/18 12:27:12 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../bits/RequestBuilder.hpp"

#include <cstddef>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main() {
	srand(time(NULL));
	int port = 7000 + (rand() % 11);
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cout << "socket failed" << std::endl;
        exit(1);
    }

    struct sockaddr_in server_addr;
    socklen_t server_addr_len = sizeof(server_addr);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&server_addr, server_addr_len) == -1) {
        std::cout << "bind failed" << std::endl;
        exit(1);
    }

    if (listen(server_fd, 1000) == -1) {
        std::cout << "listen failed" << std::endl;
        exit(1);
    }

	RequestBuilder* req = new RequestBuilder();
    std::cout << "Server is listening on localhost port: " << port << endl;
	
    while (1) {
        // Step 4: Accept a new connection
        struct sockaddr_in client_address;
        socklen_t addrlen = sizeof(client_address);
        int new_socket = accept(server_fd, (struct sockaddr*)&client_address, &addrlen);
        if (new_socket < 0) {
            std::cout << "accept failed" << std::endl;
            exit(1);
        }

        std::cout << "New connection accepted\n";

        char buffer[1024] = {0};
        // std::stringstream request;
        int bytes_received;

        while ((bytes_received = recv(new_socket, buffer, sizeof(buffer), 0)) > 0) {
            std::cout<< buffer << std::endl;
			string buf;

			ofstream ss("tmp.py", ios::app);
			ss << buf;

			buf.assign(buffer, bytes_received);
			req->build(buf);
        }

        if (bytes_received < 0) {
            std::cout << "recv failed or connection closed\n";
            close(new_socket);
            continue;
        }
    }
	delete req;
    close(server_fd);
    return 0;
}
