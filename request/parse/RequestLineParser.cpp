/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestLineParser.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:33:30 by iassil            #+#    #+#             */
/*   Updated: 2024/12/25 12:29:39 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/parse/RequestLineParser.hpp"

bool RequestLineParser::isValidRequestLine( const string& header ) {
	const string allowed =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_.~!()*"
		"'();:@&=+$,/?#[] \r";
	size_t pos = header.find_first_not_of( allowed );
	if ( pos != string::npos ) return false;
	return true;
}

void RequestLineParser::parseLine( const string& str ) {
	string		  token, something;
	istringstream tokenStream( str );

	size_t pos	= str.find( " " );
	size_t spos = str.find_first_not_of( " " );
	if ( pos < spos ) throw BAD_REQUEST;

	tokenStream >> requestLine.method >> requestLine.uri >>
		requestLine.httpVersion >> something;

	if ( !something.empty() ) throw BAD_REQUEST;
	if ( requestLine.method.empty() || requestLine.uri.empty() ||
		 requestLine.httpVersion.empty() )
		throw BAD_REQUEST;
	if ( requestLine.httpVersion != HTTP_V ) throw HTTP_VERSION;
	if ( requestLine.method != GET && requestLine.method != POST &&
		 requestLine.method != DELETE )
		throw REQUEST_IML;
	isDone = true;
}

void RequestLineParser::parse( istringstream& stream ) {
	string line;

	while ( getline( stream, line, '\n' ) ) {
		size_t pos = line.find( "\r" );
		if ( pos == string::npos ) {
			for ( size_t i = pos; i < line.size(); ++i ) {
				if ( line[i] != '\r' ) {
					throw BAD_REQUEST;
				}
			}
		}
		line = line.substr( 0, line.find( "\r" ) );
		if ( line.empty() || line == "\r" || !isValidRequestLine( line ) ) {
			throw BAD_REQUEST;
		}
		parseLine( line );
		break;
	}
}

void RequestLineParser::print() const {
	cout << YELLOW "Method: " RESET << "[" << requestLine.method << "]\n";
	cout << YELLOW "URI: " RESET << "[" << requestLine.uri << "]\n";
	cout << YELLOW "HTTP version: " RESET << "[" << requestLine.httpVersion
		 << "]\n";
}
