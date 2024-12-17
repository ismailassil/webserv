/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestLineParser.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:33:30 by iassil            #+#    #+#             */
/*   Updated: 2024/12/16 18:18:47 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../bits/parse/RequestLineParser.hpp"

bool	RequestLineParser::isValidRequestLine( const string& header ) {
	regex requestLineRegex("^([A-Z]+)([ ]+)(\\S+)([ ]+)(HTTP/\\d.\\d)([ ]*)\\r$");

	return std::regex_match(header, requestLineRegex);
}

void	RequestLineParser::parseLine( const string& str ) {
	string				token;
	std::istringstream	tokenStream(str);
	
	tokenStream >> requestLine.method >> requestLine.uri >> requestLine.httpv;

	if (requestLine.method.empty() || requestLine.uri.empty() || requestLine.httpv.empty())
		throw BAD_REQUEST;
	if (requestLine.httpv != "HTTP/1.1")
		throw HTTP_VERSION;
	if (requestLine.method != "GET" && requestLine.method != "POST" && requestLine.method != "DELETE")
		throw REQUEST_IML;
}

void	RequestLineParser::parse( istringstream& stream ) {
	string	line;

	while (getline(stream, line, '\n')) {
		if ( line.empty() || line == "\r" || !isValidRequestLine(line) ) {
			throw BAD_REQUEST;
		}
		parseLine(line);
		break ;
	}
}

void	RequestLineParser::print() {
	cout << YELLOW "Method: " RESET << "[" << requestLine.method << "]\n";
	cout << YELLOW "URI: " RESET << "[" << requestLine.uri << "]\n";
	cout << YELLOW "HTTP version: " RESET << "[" << requestLine.httpv << "]\n";
}
