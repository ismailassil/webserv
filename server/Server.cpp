#include "Server.hpp"

Server::Server() : listen( -1 ), client_max_body_size( -1 ) {
	server_names.clear();
	error_pages.clear();
	locations.clear();
}

void Server::setListen( vector< string > values ) {
	string port;

	if ( values.size() != 1 ) {
		throw runtime_error( "Error: One 'Listen Port' in Server Required" );
	}
	port = values[0];
	for ( size_t i = 0; i < port.size(); i++ ) {
		if ( !isdigit( port[i] ) ) {
			throw runtime_error( "Error: Invalid 'Listen Port'" );
		}
	}
	listen = atoi( port.c_str() );
}

void Server::setBodySize( vector< string > values ) {
	if ( values.size() != 1 ) {
		throw runtime_error( "Error: One 'Body Size' in Server Required" );
	}
	string size	  = values[0];
	size_t length = size.size();

	if ( length == 0 ) {
		throw runtime_error( "Error: Empty 'Body Size'" );
	}
	char suffix	   = size[length - 1];
	bool hasSuffix = ( suffix == 'k' || suffix == 'K' || suffix == 'm' || suffix == 'M' || suffix == 'g' || suffix == 'G' );
	for ( size_t i = 0; i < length - ( hasSuffix ? 1 : 0 ); i++ ) {
		if ( !isdigit( size[i] ) ) {
			throw runtime_error( "Error: Invalid 'Body Size' format near '" + size + "'" );
		}
	}
	int numericValue = atoi( size.c_str() );
	if ( numericValue < 0 ) {
		throw runtime_error( "Error: Negative 'Body Size' not allowed" );
	}
	if ( hasSuffix ) {
		switch ( suffix ) {
			case 'k':
			case 'K':
				numericValue *= 1024;
				break;
			case 'm':
			case 'M':
				numericValue *= 1024 * 1024;
				break;
			case 'g':
			case 'G':
				numericValue *= 1024 * 1024 * 1024;
				break;
			default: {
				throw runtime_error( "Error: Unknown Unit in 'Body Size'" );
			}
		}
	}
	if ( numericValue == 0 )
		numericValue = -1;
	client_max_body_size = numericValue;
}

void Server::setServerName( vector< string > values ) {
	for ( size_t i = 0; i < values.size(); i++ )
		server_names.push_back( values[i] );
}

void Server::setErrorPages( const vector< string > values ) {
	if ( values.size() % 2 != 0 ) {
		throw runtime_error( "Error: Mismatched error codes and pages in 'error_page' directive" );
	}
	for ( size_t i = 0; i < values.size(); i += 2 ) {
		const string& errorCode = values[i];
		const string& errorPage = values[i + 1];
		for ( size_t j = 0; j < errorCode.size(); j++ ) {
			if ( !isdigit( errorCode[j] ) ) {
				throw runtime_error( "Error: Invalid error code '" + errorCode + "' in 'error_page' directive" );
			}
		}
		error_pages[atoi( errorCode.c_str() )] = errorPage;
	}
}

void Server::addLocation( Location location ) {
	locations.push_back( location );
}

void Server::setDirectives( map< string, vector< string > > directives ) {
	for ( map< string, vector< string > >::iterator iter = directives.begin(); iter != directives.end(); iter++ ) {
		if ( iter->first == "listen" )
			setListen( iter->second );
		else if ( iter->first == "client_max_body_size" )
			setBodySize( iter->second );
		else if ( iter->first == "server_name" )
			setServerName( iter->second );
		else if ( iter->first == "error_pages" )
			setErrorPages( iter->second );
		else
			throw runtime_error( "Error: Invalid token '" + iter->first + "' in Server" );
	}
}

int Server::getListen() const {
	return listen;
}

vector< string >& Server::getServerNames() {
	return server_names;
}

map< int, string >& Server::getErrorPages() {
	return error_pages;
}

int Server::getClientMaxBodySize() const {
	return client_max_body_size;
}

vector< Location >& Server::getLocations() {
	return locations;
}

void Server::setListen( int listen ) {
	this->listen = listen;
}

void Server::setServerNames( const vector< string >& serverNames ) {
	this->server_names = serverNames;
}

void Server::setErrorPages( const map< int, string >& errorPages ) {
	this->error_pages = errorPages;
}

void Server::setClientMaxBodySize( int clientMaxBodySize ) {
	this->client_max_body_size = clientMaxBodySize;
}

void Server::setLocations( const vector< Location >& locations ) {
	this->locations = locations;
};