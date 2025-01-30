#include "ConfigParser.hpp"

ConfigParser::ConfigParser() {}

void ConfigParser::tokenize( const string& configContent, vector< string >& tokens ) {
	string currentToken;

	for ( size_t i = 0; i < configContent.size(); i++ ) {
		char c = configContent[i];
		if ( c == '#' ) {
			while ( i < configContent.size() && configContent[i] != '\n' )
				i++;
		} else if ( c == '{' || c == '}' || c == ';' ) {
			if ( !currentToken.empty() ) {
				tokens.push_back( currentToken );
				currentToken.clear();
			}
			tokens.push_back( string( 1, c ) );
			if ( c == ';' )
				while ( i < configContent.size() && configContent[i] == ';' )
					i++;
		} else if ( isspace( c ) || c == '\n' ) {
			if ( !currentToken.empty() ) {
				tokens.push_back( currentToken );
				currentToken.clear();
			}
			// while (i < configContent.size() && isspace(configContent[i]))
			//     i++;
		} else
			currentToken += c;
	}
	if ( !currentToken.empty() ) {
		tokens.push_back( currentToken );
	}
}

bool ConfigParser::isValidPath( const string& path ) {
	if ( path.empty() || path[0] != '/' )
		return false;
	for ( size_t i = 0; i < path.size(); i++ ) {
		if ( path[i] == ' ' || path[i] == '\t' ) {
			return false;
		}
	}

	return true;
}

int ConfigParser::extractLocationContext( const vector< string >& tokens, int startIndex, Location& location ) {
	map< string, vector< string > > directives;

	for ( size_t i = startIndex; i < tokens.size(); i++ ) {
		if ( tokens[i] == "}" ) {
			location.setDirectives( directives );
			return i;
		} else {
			string key = tokens[i];
			if ( i + 1 < tokens.size() ) {
				i++;
				size_t j = i;
				while ( i < tokens.size() && tokens[i] != ";" && tokens[i] != "{" && tokens[i] != "}" ) {
					directives[key].push_back( tokens[i] );
					i++;
				}
				if ( j == i || i >= tokens.size() || tokens[i] != ";" ) {
					throw runtime_error( "Error: Missing ';' after values for key '" + key + "'" );
				}
			}

			else {
				throw runtime_error( "Error: Invalid directive syntax near 1 '" + key + "'" );
			}
		}
	}

	{
		throw runtime_error( "Error: Missing '}' to close 'location' context." );
	}
}

int ConfigParser::extractServerContext( const vector< string >& tokens, int startIndex, Server& server ) {
	map< string, vector< string > > directives;

	for ( size_t i = startIndex; i < tokens.size(); i++ ) {
		if ( tokens[i] == "}" ) {
			server.setDirectives( directives );
			return i;
		} else if ( tokens[i] == "location" ) {
			if ( i + 3 < tokens.size() && tokens[i + 1] != "{" && tokens[i + 2] == "{" ) {
				string locationPath = tokens[i + 1];
				if ( !isValidPath( locationPath ) ) {
					throw runtime_error( "Error: Invalid path for 'location': " + locationPath );
				}
				Location location( locationPath );
				i = extractLocationContext( tokens, i + 3, location );
				server.addLocation( location );
			} else {
				throw runtime_error( "Error: Invalid syntax for 'location'" );
			}
		} else {
			string key = tokens[i];
			if ( i + 1 < tokens.size() ) {
				i++;
				size_t j = i;
				while ( i < tokens.size() && tokens[i] != ";" && tokens[i] != "{" && tokens[i] != "}" ) {
					directives[key].push_back( tokens[i] );
					i++;
				}
				if ( j == i || i >= tokens.size() || tokens[i] != ";" ) {
					throw runtime_error( "Error: Missing ';' after values for key '" + key + "'" );
				}
			}

			else {
				throw runtime_error( "Error: Invalid directive syntax near 2 '" + key + "'" );
			}
		}
	}
	{
		throw runtime_error( "Error: Missing '}' to close 'server' context." );
	}
}

void ConfigParser::parseTokens( const vector< string >& tokens, vector< Server >& servers ) {
	bool serverFound = false;

	for ( size_t i = 0; i < tokens.size(); i++ ) {
		if ( tokens[i] == "server" ) {
			Server currentServer;
			if ( i + 1 < tokens.size() && tokens[i + 1] == "{" ) {
				serverFound = true;
				i			= extractServerContext( tokens, i + 2, currentServer );
				servers.push_back( currentServer );
			} else {
				throw runtime_error( "Error: 'server' must be followed by '{'" );
			}
		} else {
			throw runtime_error( "Error: Unknown context" );
		}
	}

	if ( !serverFound )
		throw runtime_error( "Error: No 'server' context found in the configuration." );
}

void ConfigParser::checkConfigValidity( vector< Server >& servers ) {
	set< int > serverPorts;

	for ( vector< Server >::iterator serverIt = servers.begin(); serverIt != servers.end(); ++serverIt ) {
		if ( serverIt->getListen() == -1 ) {
			cerr << "Warning: 'listen' directive missing. Defaulting to port 80.\n";
			serverIt->setListen( 80 );
		} else if ( serverIt->getListen() < 1 || serverIt->getListen() > 65535 ) {
			throw runtime_error( "Error: Invalid port '" + intToString( serverIt->getListen() ) + "' in 'listen' directive. Must be between 1 and 65535." );
		}

		if ( serverPorts.find( serverIt->getListen() ) != serverPorts.end() ) {
			throw runtime_error( "Error: Duplicate 'listen' port '" + intToString( serverIt->getListen() ) + "' found in configuration." );
		}
		serverPorts.insert( serverIt->getListen() );

		if ( serverIt->getServerNames().empty() ) {
			cerr << "Warning: No 'server_name' specified. Defaulting to 'default_server'.\n";
			serverIt->getServerNames().push_back( "default_server" );
		}
		if ( serverIt->getLocations().empty() ) {
			throw runtime_error( "Error: No 'location' context found in the configuration for server listening on port '" + intToString( serverIt->getListen() ) + "'." );
		}

		set< string >		locationPaths;
		vector< Location >& locations = serverIt->getLocations();
		for ( vector< Location >::iterator locIt = locations.begin(); locIt != locations.end(); ++locIt ) {
			if ( locIt->getPath().empty() )
				throw runtime_error( "Error: 'location' context has an empty path in server listening on port '" + intToString( serverIt->getListen() ) + "'." );

			if ( locationPaths.find( locIt->getPath() ) != locationPaths.end() )
				throw runtime_error( "Error: Duplicate 'location' path '" + locIt->getPath() + "' found in server listening on port '" + intToString( serverIt->getListen() ) + "'." );

			locationPaths.insert( locIt->getPath() );
		}
	}
}

void ConfigParser::parseConfig( const string& config_file, vector< Server >& servers ) {
	string			 line;
	string			 fileContent;
	vector< string > tokens;
	ifstream		 file( config_file.c_str() );

	if ( !file.is_open() )
		throw runtime_error( "Error: Can't Open Configuration File" );

	while ( getline( file, line ) ) {
		fileContent += line;
		if ( !file.eof() )
			fileContent += '\n';
	}
	tokenize( fileContent, tokens );
	parseTokens( tokens, servers );
	checkConfigValidity( servers );
}