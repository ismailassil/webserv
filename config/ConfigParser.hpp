#pragma once

#include "../server/Server.hpp"
#include "../srcs/bits.hpp"	 // IWYU pragma: keep

class ConfigParser {
	private:
		void tokenize( const string& configContent, vector< string >& tokens );
		void parseTokens( const vector< string >& tokens, vector< Server >& servers );
		int	 extractLocationContext( const vector< string >& tokens, int startIndex, Location& location );
		int	 extractServerContext( const vector< string >& tokens, int startIndex, Server& server );
		bool isValidPath( const string& path );
		void checkConfigValidity( vector< Server >& servers );

	public:
		ConfigParser();
		void parseConfig( const string& config_file, vector< Server >& servers );
};