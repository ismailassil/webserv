/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:07:39 by iassil            #+#    #+#             */
/*   Updated: 2024/12/16 11:18:40 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream> // IWYU pragma: keep
#include <map> // IWYU pragma: keep
#include <regex> // IWYU pragma: keep

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::map;
using std::istringstream;
using std::istream;
using std::getline;
using std::find;
using std::regex;

struct requestLine {
	string method;
	string uri;
	string httpv;
};

class RequestParser {
	protected:
		requestLine			requestLine;
		map<string, string>	headers;
		string				body;

	public:
		virtual ~RequestParser() {}
		virtual void	parse( istringstream& stream ) = 0;
		virtual void	print() = 0;

		struct requestLine&		getRequestLine();
		map<string, string>&	getHeaders();
		string&					getBody();
		
};