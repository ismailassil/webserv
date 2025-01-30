#include "WebServer.hpp"

#include "bits.hpp"

WebServer::WebServer() {}

void WebServer::init( const string& config_file ) {
	configParser.parseConfig( config_file, servers );
}

void WebServer::run() {
	// create a socket
	// bind the socket
	// listen on the socket
	// accept connections
	// read/write data
	// close the connection
	/////////////////////////////
	// Setup kqueue
	// Register all server sockets with kqueue
	// Loop until shutdown
	// Handle events
	// Accept new connections
	// Handle data from client sockets
}

void WebServer::shutdown() {
	// Close the server
	// Close all connections
}

void WebServer::displayData() {
	int serverCount = 1;

	for ( vector< Server >::iterator serverIt = servers.begin(); serverIt != servers.end(); ++serverIt, ++serverCount ) {
		cout << "Server " << serverCount << ":\n";

		cout << "  Listen: " << serverIt->getListen() << "\n";
		cout << "  Server Names: ";
		vector< string > serverNames = serverIt->getServerNames();
		for ( vector< string >::iterator nameIt = serverNames.begin(); nameIt != serverNames.end(); ++nameIt ) {
			if ( nameIt != serverNames.begin() ) cout << ", ";
			cout << *nameIt;
		}
		cout << "\n";

		cout << "  Client Max Body Size: ";
		if ( serverIt->getClientMaxBodySize() > 0 )
			cout << serverIt->getClientMaxBodySize() << " bytes\n";
		else
			cout << "Unlimited\n";

		cout << "  Error Pages:\n";
		map< int, string > errorPages = serverIt->getErrorPages();
		for ( map< int, string >::iterator errorIt = errorPages.begin(); errorIt != errorPages.end(); ++errorIt ) {
			cout << "    " << errorIt->first << " -> " << errorIt->second << "\n";
		}

		cout << "  Locations:\n";
		int				   locationCount = 1;
		vector< Location > locations	 = serverIt->getLocations();
		for ( vector< Location >::iterator locIt = locations.begin(); locIt != locations.end(); ++locIt, ++locationCount ) {
			cout << "    Location " << locationCount << ":\n";

			cout << "      Path: " << locIt->getPath() << "\n";
			cout << "      Root: " << locIt->getRoot() << "\n";
			cout << "      Index: " << locIt->getIndex() << "\n";
			cout << "      Autoindex: " << ( locIt->getAutoindex() ? "on" : "off" ) << "\n";
			cout << "      Upload Path: " << locIt->getUploadPath() << "\n";
			cout << "      Redirect: " << locIt->getRedirect() << "\n";

			cout << "      Methods: ";
			set< string > methods = locIt->getMethods();
			for ( set< string >::iterator methodIt = methods.begin(); methodIt != methods.end(); ++methodIt ) {
				if ( methodIt != methods.begin() ) cout << ", ";
				cout << *methodIt;
			}
			cout << "\n";
		}
		cout << "\n";
	}
}
