/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:16:53 by iassil            #+#    #+#             */
/*   Updated: 2024/12/16 18:37:57 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "RequestParser.hpp"
#include "parse/RequestLineParser.hpp" // IWYU pragma: keep
#include "parse/HeaderParser.hpp" // IWYU pragma: keep
#include "parse/BodyParser.hpp" // IWYU pragma: keep
#include <iostream> // IWYU pragma: keep
#include <map> // IWYU pragma: keep
#include <sstream>
#include <cstring> // IWYU pragma: keep
#include <string>
#include <regex>
#include <algorithm>

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

#define REQUEST_IML "501 Not Implemented\n"
#define BAD_REQUEST "400 Bad Request\n"

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
using std::vector;

class BodyParser;

enum req_stat {
	CHUNCKED,
	CHUNCK_BOUND,
	BOUNDARIES,
	CONTENT_LENGTH,
	NONE
};

/*
BODY - Handle
	-> Transfer-encoding
		-> chuncked (vanilla)
			-> chuncked
			-> chuncked with boundaries
		-> otherwise: 501 Not Implemented
	-> boundaries
	-> Content-length

*/

class Request {
	private:
		RequestLineParser*		requestLineParser;
		HeaderParser*			headerParser;
		BodyParser*				bodyParser;

		vector<RequestParser*>	request;
		req_stat				req_status;
		t_header_infos			header_infos;

		Request& operator=( const Request& );
		Request( const Request& );
	
	public:
		Request();
		~Request();

		void			parseRequestHeader( const string& RawRequest );
		void			parseRequestBody( const string& RawRequest, t_header_infos& header_info );

		req_stat		getRequestStatus();
		t_header_infos	getHeaderInfos();

		void	print();
};
