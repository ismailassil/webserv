
#include <iostream>
///// INCLUDE HEADERS
#include <arpa/inet.h>
#include <arpa/inet.h>	// Include the arpa/inet.h header file
#include <errno.h>
#include <netdb.h>	// Include the netdb.h header file
#include <netinet/in.h>
#include <stdio.h>
#include <sys/_types/_ssize_t.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>  // IWYU pragma: keep
#include <cerrno>
#include <cstddef>	// IWYU pragma: keep
#include <cstring>	// IWYU pragma: keep
#include <cstring>
#include <fstream>	 // IWYU pragma: keep
#include <ios>		 // IWYU pragma: keep
#include <iostream>	 // IWYU pragma: keep
#include <map>		 // IWYU pragma: keep
#include <regex>	 // IWYU pragma: keep
#include <sstream>	 // IWYU pragma: keep
#include <stdexcept>
#include <string>	// IWYU pragma: keep
#include <utility>	// IWYU pragma: keep
using std::cout;
using std::endl;
using std::ifstream;
using std::ios;
using std::istream;
using std::istringstream;
using std::ofstream;
using std::runtime_error;
using std::string;
using std::stringstream;

#define PORT		8080
#define IP			"127.0.0.1"
#define BUFFER_SIZE 8192

class Server {
	private:
		int				   socketfd;
		int				   bindSocket;
		int				   connectSocket;
		int				   backlog;
		int				   lt;
		struct sockaddr_in address;
		int				   getProtocolName( const char *name );
		int				   getService( const char *name );

	public:
		Server( int, const char *, int, int );
		void startServer();
};

int Server::getProtocolName( const char *name ) {
	struct protoent *proto = getprotobyname( name );
	if ( proto == NULL ) throw runtime_error( "Invalid protocol name" );

	return proto->p_proto;
}

int Server::getService( const char *name ) {
	if ( !std::strncmp( "tcp", name, 3 ) )
		return SOCK_STREAM;
	else if ( !std::strncmp( "udp", name, 3 ) )
		return SOCK_DGRAM;
	else
		throw runtime_error( "Unsupported protocol" );
}

Server::Server( int domain, const char *service, int port, int backlog ) {
	// Define Address Struct
	address.sin_family = domain;
	address.sin_port   = htons( port );
	/* address.sin_addr.s_addr = inet_addr( IP ); */
	address.sin_addr.s_addr = INADDR_ANY;
	std::memset( address.sin_zero, '\0', sizeof( address.sin_zero ) );

	// Get the protocol type
	int protocol = getProtocolName( service );
	int serv	 = getService( service );

	// Establish Connection
	socketfd = socket( domain, serv, protocol );
	if ( socketfd == -1 )
		throw runtime_error( string( "socket - " ) + strerror( errno ) );

	// The reuse of the address
	int reuse	  = 1;
	int socketOpt = setsockopt( socketfd, SOL_SOCKET, SO_REUSEADDR, &reuse,
								sizeof( reuse ) );
	if ( socketOpt == -1 )
		throw runtime_error( string( "setsockopt - " ) + strerror( errno ) );

	// Bind
	bindSocket =
		bind( socketfd, (struct sockaddr *)&address, sizeof( address ) );
	if ( bindSocket == -1 )
		throw runtime_error( string( "bind - " ) + strerror( errno ) );

	// Listening to the Socket
	lt = listen( socketfd, backlog );
	if ( lt == -1 )
		throw runtime_error( string( "listen - " ) + strerror( errno ) );
}

void Server::startServer() {
	while ( true ) {
		char buffer[BUFFER_SIZE] = { 0 };

		socklen_t addr_len = sizeof( address );

		cout << "========== Waiting for Connection ==========" << endl;

		int newSock =
			accept( socketfd, (struct sockaddr *)&address, &addr_len );
		if ( newSock == -1 )
			throw runtime_error( string( "accept - " ) + strerror( errno ) );

		cout << "========== Connection Accepted ==========" << endl;
		ssize_t bytes_received = recv( newSock, &buffer, BUFFER_SIZE, 0 );
		if ( bytes_received == -1 )
			throw runtime_error( string( "recv - " ) + strerror( errno ) );
		if ( bytes_received == 0 ) {
			cout << "Client Disconnected" << endl;
			close( newSock );
			continue;
		}

		// ssize_t bytes_sent =
		// 	send( newSock, http_response.c_str(), http_response.size(), 0 );
		// if ( bytes_sent == -1 ) {
		// 	cerr << "send - " << strerror( errno ) << endl;
		// } else {
		// 	cout << "HTML file sent to client." << endl;
		// }
	}
}

// int main() {
// 	try {
// 		Server sk = Server( AF_INET, "tcp", PORT, 5 );
// 		sk.startServer();
// 	} catch ( const exception &e ) {
// 		cout << "Error: " << e.what() << endl;
// 	}
// 	////
// }