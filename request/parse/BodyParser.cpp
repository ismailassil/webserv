/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BodyParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 15:22:27 by iassil            #+#    #+#             */
/*   Updated: 2024/12/16 18:55:39 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../bits/parse/BodyParser.hpp"

void	BodyParser::parse( istringstream& stream ) {
	
}

void	BodyParser::parseChunckedBody( istringstream& stream, t_header_infos& header_info ) {
	string	line;
	char	*end;
	long	length;

	while ( !isChuncked && getline(stream, line, '\n') ) {
		size_t	pos = line.find_first_not_of("1234567890\r");
		if ( pos == std::string::npos && line == "\r\n" )
			continue ;
		else
		 	throw BAD_REQUEST;
		length = strtol(line.c_str(), &end, 16) + 2;
		if (*end != '\r')
			throw BAD_REQUEST;
		isChuncked = true;
	}
	if ( getline(stream, line) ) {
		
	}
	
}

void	BodyParser::print() {
	
}
