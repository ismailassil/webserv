/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeaderParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:38:45 by iassil            #+#    #+#             */
/*   Updated: 2024/12/25 10:28:48 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/parse/HeaderParser.hpp"

bool HeaderParser::isValidHeader( const string& line ) {
	size_t crPos = line.find( "\r" );
	for ( size_t i = crPos; i < line.size(); ++i ) {
		if ( line[i] != '\r' ) {
			throw BAD_REQUEST;
		}
	}
	const string str = line.substr( 0, crPos );
	size_t		 i	 = 0;
	for ( ; i < str.size() && str[i] != ':'; i++ ) {
		if ( !std::isprint( str[i] ) || str[i] == ' ' ) throw BAD_REQUEST;
	}
	if ( str[i] == '\0' ) return true;
	for ( ; i < str.size(); i++ ) {
		if ( !std::isprint( str[i] ) && str[i] != '\t' ) throw BAD_REQUEST;
	}
	if ( str[i] == '\0' ) return true;
	return false;
}

bool HeaderParser::isDoubleCRLF( istream& stream, const string& line ) {
	size_t pos = line.find( "\r" );

	char s = stream.peek();
	if ( pos != string::npos && s == '\r' && line.length() - 1 == pos )
		return true;

	return false;
}

void HeaderParser::parseLine( const string& line ) {
	string field, value;
	size_t pos = line.find( ':' );

	if ( pos == string::npos ) {
		pos = line.find( "\r" );
	} else {
		int pcr = line.find( "\r" );
		value	= line.substr( pos + 1, pcr - pos + 1 );

		size_t epos = value.find_first_not_of( " \t\r" );
		if ( epos != string::npos ) {
			value.erase( 0, epos );
			size_t spos = value.find_last_not_of( " \t\r" );
			if ( spos < value.length() ) value.erase( spos + 1 );
		}
	}
	field = line.substr( 0, pos );
	if ( field.empty() ) throw BAD_REQUEST;
	transform( field.begin(), field.end(), field.begin(), ::tolower );
	if ( field == "transfer-encoding" || field == "content-type" )
		transform( value.begin(), value.end(), value.begin(), ::tolower );
	if ( field == "host" && value.empty() ) throw BAD_REQUEST;
	headers[field] = value;
}

void HeaderParser::parse( istringstream& stream ) {
	string line;

	while ( getline( stream, line, '\n' ) ) {
		if ( line.empty() || line == "\r" || !isValidHeader( line ) )
			throw BAD_REQUEST;
		parseLine( line );
		if ( isDoubleCRLF( stream, line ) ) break;
	}
	if ( headers.find( HOST ) == headers.end() ) throw BAD_REQUEST;
}

void HeaderParser::print() const {
	map< string, string >::const_iterator it = headers.begin();
	for ( ; it != headers.end(); it++ ) {
		cout << YELLOW << it->first << ": " RESET << "[" << it->second << "]\n";
	}
}