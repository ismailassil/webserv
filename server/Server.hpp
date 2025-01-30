#pragma once

#include "../config/Location.hpp"

class Server {
	private:
		int				   listen;
		int				   client_max_body_size;
		vector< string >   server_names;
		map< int, string > error_pages;
		vector< Location > locations;

	public:
		Server();
		void setDirectives( map< string, vector< string > > directives );
		void setListen( vector< string > values );
		void setBodySize( vector< string > values );
		void setServerName( vector< string > values );
		void setErrorPages( vector< string > values );
		void addLocation( Location location );

		// Getters
		int					getListen() const;
		int					getClientMaxBodySize() const;
		vector< string >&	getServerNames();
		map< int, string >& getErrorPages();
		vector< Location >& getLocations();

		// Setters
		void setListen( int listen );
		void setClientMaxBodySize( int clientMaxBodySize );
		void setServerNames( const vector< string >& serverNames );
		void setErrorPages( const map< int, string >& errorPages );
		void setLocations( const vector< Location >& locations );
};
