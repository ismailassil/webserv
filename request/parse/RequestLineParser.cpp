/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestLineParser.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:33:30 by iassil            #+#    #+#             */
/*   Updated: 2024/12/17 12:39:32 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../bits/parse/RequestLineParser.hpp"

bool	RequestLineParser::isValidRequestLine( const string& header ) {
	regex requestLineRegex("^([A-Z]+)([ ]+)(\\S+)([ ]+)(HTTP/\\d.\\d)([ ]*)\\r$");

	return std::regex_match(header, requestLineRegex);
}

void	RequestLineParser::parseLine( const string& str ) {
	string				token, something;
	std::istringstream	tokenStream(str);
	
	tokenStream >> requestLine.method >> requestLine.uri >> requestLine.httpVersion >> something;

	if (requestLine.method.empty() || requestLine.uri.empty() || requestLine.httpVersion.empty())
		throw BAD_REQUEST;
	if (requestLine.httpVersion != HTTP_V)
		throw HTTP_VERSION;
	if (requestLine.method != GET && requestLine.method != POST && requestLine.method != DELETE)
		throw REQUEST_IML;
	isDone = true;
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

void	RequestLineParser::print() const {
	cout << YELLOW "Method: " RESET << "[" << requestLine.method << "]\n";
	cout << YELLOW "URI: " RESET << "[" << requestLine.uri << "]\n";
	cout << YELLOW "HTTP version: " RESET << "[" << requestLine.httpVersion << "]\n";
}
