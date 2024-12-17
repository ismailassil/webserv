/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeaderParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:38:45 by iassil            #+#    #+#             */
/*   Updated: 2024/12/16 15:22:55 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../bits/parse/HeaderParser.hpp"

bool	HeaderParser::isValidHeader( const string& line ) {
    std::regex regexHeader("^([A-Za-z0-9\\-]+):([ \t]*)([^\\r\\n]+)\\r$");

	return std::regex_match(line, regexHeader);
}

void	HeaderParser::lowerString( string& str ) {
	for ( int i = 0; i < static_cast<int>(str.length()); i++ ) {
		str[i] = tolower(str[i]);
	}
}

bool	HeaderParser::isDoubleCRLF( istream& stream, const string& line ) {
	size_t	pos = line.find("\r");

	char s = stream.peek();
	if ( pos != std::string::npos && s == '\r' && line.length() - 1 == pos )
		return true;

	return false;
}

void	HeaderParser::parseLine(const string& line) {
	string field, value;
	size_t pos = line.find(':');

	if (pos == std::string::npos)
		throw BAD_REQUEST;

	field = line.substr(0, pos);
	value = line.substr(pos + 1);
	value.erase(0, value.find_first_not_of(" \t"));
	value.erase(value.find_last_not_of(" \t"));
	if ( field.empty() )
		throw BAD_REQUEST;
	lowerString(field);
	headers[field] = value;
}

void	HeaderParser::parse( istringstream& stream ) {
	string	line;

	while (getline(stream, line, '\n')) {
		if ( line.empty() || line == "\r" || !isValidHeader(line) )
			throw BAD_REQUEST;
		parseLine(line);
		if ( isDoubleCRLF(stream, line) )
			break ;
	}
	if (headers.find(HOST) == headers.end())
		throw BAD_REQUEST;
}

void	HeaderParser::print() {
	map<string, string>::iterator it = headers.begin();
	for ( ; it != headers.end(); it++) {
		cout << YELLOW << it->first << ": " RESET << "[" << it->second << "]\n";
	}
}