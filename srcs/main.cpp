#include "WebServer.hpp"
#include "bits.hpp"	 // IWYU pragma: keep

void checkConfigFile( const string &config_file ) {
	size_t pos = config_file.find( ".conf" );
	if ( pos == string::npos ) {
		cerr << "Invalid config file" << endl;
		exit( 1 );
	}
	if ( pos != config_file.size() - 5 ) {
		cerr << "Invalid config file" << endl;
		exit( 1 );
	}
}

int main( int ac, char **av ) {
	if ( ac != 2 )
		return cerr << "Usage: ./webserv <config_file>" << endl, 1;
	checkConfigFile( av[1] );

	WebServer webserver;
	try {
		webserver.init( av[1] );
	} catch ( const exception &e ) {
		cerr << e.what() << endl;
		exit( 1 );
	}
	webserver.displayData();

	int sock_fd = socket( AF_INET, SOCK_STREAM, 0 );
	if ( sock_fd == -1 ) {
		cerr << "Socket Creation Error" << endl;
		exit( 1 );
	}
	sockaddr_in sockaddr;
	sockaddr.sin_family		 = AF_INET;
	sockaddr.sin_addr.s_addr = INADDR_ANY;
	sockaddr.sin_port		 = htons( 9999 );
	if ( bind( sock_fd, (struct sockaddr *)&sockaddr, sizeof( sockaddr ) ) < 0 ) {
		cerr << "Failed to bind to port 9999. errno: " << errno << endl;
		exit( 1 );
	}
	if ( listen( sock_fd, 10 ) < 0 ) {
		cerr << "Failed to listen on socket. errno: " << errno << endl;
		exit( 1 );
	}
	unsigned long addrlen	 = sizeof( sockaddr );
	int			  connection = accept( sock_fd, (struct sockaddr *)&sockaddr, (socklen_t *)&addrlen );
	if ( connection < 0 ) {
		cout << "Failed to grab connection. errno: " << errno << endl;
		exit( EXIT_FAILURE );
	}

	char buffer[BUFFER_SIZE];
	recv( connection, buffer, sizeof( buffer ), 0 );
	cout << "The message was: " << buffer;

	string response = "Good talking to you\n";
	send( connection, response.c_str(), response.size(), 0 );

	close( connection );
	close( sock_fd );
}
