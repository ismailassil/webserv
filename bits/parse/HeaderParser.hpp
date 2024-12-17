/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeaderParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:29:17 by iassil            #+#    #+#             */
/*   Updated: 2024/12/16 14:52:08 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../RequestParser.hpp" // IWYU pragma: keep
#include <iostream> // IWYU pragma: keep
#include <map> // IWYU pragma: keep
#include <sstream> // IWYU pragma: keep
#include <cstring> // IWYU pragma: keep
#include <string> // IWYU pragma: keep
#include <regex> // IWYU pragma: keep
#include <algorithm> // IWYU pragma: keep

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

#define BAD_REQUEST "400 Bad Request\n"
#define HOST		"host"

class HeaderParser : public RequestParser {
	private:
		bool	isValidHeader( const string& header );
		void	parseLine( const string& str );
		bool	isDoubleCRLF( istream& stream, const string& line );
		void	lowerString( string& str );

	public:
		void	parse( istringstream& stream );
		void	print();
};