/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 15:36:29 by iassil            #+#    #+#             */
/*   Updated: 2024/12/27 15:45:36 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstddef>

#include "../headers/RequestBuilder.hpp"
#include "../headers/bits.hpp"
// #include <exception>
#include <cstring>
#include <iostream>
// #include <fstream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT		7000
#define BUFFER_SIZE 8192

int main() {
	srand( time( NULL ) );

	int server_fd = socket( AF_INET, SOCK_STREAM, 0 );
	if ( server_fd < 0 ) {
		cout << "socket failed" << endl;
		exit( 1 );
	}

	int yes = 1;
	setsockopt( server_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof( yes ) );

	struct sockaddr_in server_addr;
	socklen_t		   server_addr_len = sizeof( server_addr );
	server_addr.sin_family			   = AF_INET;
	server_addr.sin_addr.s_addr		   = inet_addr( "127.0.0.1" );
	server_addr.sin_port			   = htons( PORT );

	if ( bind( server_fd, (struct sockaddr*)&server_addr, server_addr_len ) ==
		 -1 ) {
		cout << "bind failed" << endl;
		exit( 1 );
	}

	if ( listen( server_fd, 1000 ) == -1 ) {
		cout << "listen failed" << endl;
		exit( 1 );
	}

	cout << "Server is listening on localhost port: " << PORT << endl;

	while ( 1 ) {
		RequestBuilder	   req;
		struct sockaddr_in client_address;
		socklen_t		   addrlen = sizeof( client_address );
		int				   new_socket =
			accept( server_fd, (struct sockaddr*)&client_address, &addrlen );
		if ( new_socket < 0 ) {
			cout << "accept failed" << endl;
			exit( 1 );
		}

		cout << "=================START=================" << endl;

		char buffer[BUFFER_SIZE] = { 0 };
		int	 bytes_received;
		// double start;

		while ( ( bytes_received =
					  recv( new_socket, buffer, sizeof( buffer ), 0 ) ) > 0 ) {
			string buf;
			buf.assign( buffer, bytes_received );
			if ( bytes_received == 0 )
				cout << "=================END=================" << endl;

			// // /////////////////////////////////////////////////////
			// ofstream ss( "request.py", ios::app | ios::binary );
			// ofstream sss( "chunks.py", ios::app | ios::binary );
			// // /////////////////////////////////////////////////////////
			// ss.write( buf.c_str(), buf.length() );

			// sss << "\n=================== Step: [" << step
			// 	<< "] ========================================\n";
			// sss.write( buf.c_str(), buf.length() );
			// start = clock();
			// sleep(1);

			try {
				/////////////////////////////////////////
				/////////////////////////////////////////
				/////////////////////////////////////////
				/*********/ req.build( buf ); /*********/
				/////////////////////////////////////////
				/////////////////////////////////////////
				/////////////////////////////////////////
			} catch ( const char* e ) {
				cerr << "Error: " << e << endl;
				exit( 1 );
			} catch ( const exception& ee ) {
				cerr << "Error: " << ee.what() << endl;
				exit( 1 );
			}
			cout << "=======" << step++ << "=======" << endl;
		}
		req.print();
		// cout << "TIME: " << (static_cast<double>(clock()) - start) /
		// CLOCKS_PER_SEC  << "s" << endl;
		cout << "=================END=================" << endl;

		if ( bytes_received < 0 ) {
			cout << "recv failed or connection closed\n";
			close( new_socket );
			continue;
		}
	}
	close( server_fd );
	return 0;
}
