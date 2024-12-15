/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 15:58:58 by iassil            #+#    #+#             */
/*   Updated: 2024/12/15 19:40:14 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../bits/Request.hpp"

bool	Request::isValidHeader( const string& line ) {
	std::regex	regexHeader("^([A-Za-z0-9\\-]+):[ \t]*([ ~]+\\r)$");

	return std::regex_match(line, regexHeader);
}

bool	Request::isValidRequestLine( const string& header ) {
	regex requestLineRegex("^([A-Z]+) (\\S+) (HTTP/\\d\\.\\d)\\r$");

	return std::regex_match(header, requestLineRegex);
}


void	Request::carriageRegex( const string& line ) {
	size_t	pos = line.find("\r");
	
	if (pos == std::string::npos || pos != line.length() - 1)
		throw BAD_REQUEST;
}

void Request::extractHeaders(const string &line) {
	string field, value;
	size_t pos = line.find(':');

	if (pos == std::string::npos)
		throw BAD_REQUEST;

	field = line.substr(0, pos);
	value = line.substr(pos + 1);
	value.erase(0, value.find_first_not_of(" \t"));
	value.erase(value.find_last_not_of(" \t"));
	if (field.empty() || value.empty())
		throw BAD_REQUEST;
	headers[field] = value;
}

bool	Request::isDoubleCRLF( istream& stream, const string& line ) {
	size_t	pos = line.find("\r");

	char s = stream.peek();
	if ( pos != std::string::npos && s == '\r' && line.length() - 1 == pos )
		return true;

	return false;
}

Request::Request( const string& request ) {
	istringstream	stream(request);
	string			line;

	while (getline(stream, line, '\n')) {
		if ( line.empty() || line == "\r" || !isValidRequestLine(line) ) {
			throw BAD_REQUEST;
		}
		_req_line_.split_string(line);
		carriageRegex(line);
		break ;
	}

	while (getline(stream, line, '\n')) {
		if ( isDoubleCRLF(stream, line) )
			break ;
		if ( line.empty() || line == "\r" || isValidHeader(line) )
			throw BAD_REQUEST;
		extractHeaders(line);
	}
}