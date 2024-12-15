/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 15:51:17 by iassil            #+#    #+#             */
/*   Updated: 2024/12/15 19:51:52 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
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

class Request {
	private:
		struct req_ {
			string method;
			string uri;
			string httpv;

			void	split_string( const std::string& str ) {
				string				token;
				std::istringstream	tokenStream(str);
				
				if (getline(tokenStream, token, ' ')) {
					method = token;
				}
				
				if (getline(tokenStream, token, ' ')) {
					uri = token;
				}
				
				if (getline(tokenStream, token, '\r')) {
					httpv = token;
				}
				
				size_t	pos = httpv.find_first_of(" \t");
				if (pos != std::string::npos || method.empty() \
					|| uri.empty() || httpv.empty())
					throw BAD_REQUEST;
			}
		};

		req_				_req_line_;
		map<string, string>	headers;

		Request() {}
		Request& operator=( const Request& obj ) {
			(void)obj;
			return *this;
		}

		bool	isValidHeader( const string& header );
		bool	isValidRequestLine( const string& header );
		void	carriageRegex( const string& line );
		void 	extractHeaders(const string &line);
		bool	isDoubleCRLF( istream& stream, const string& line );
	
	public:
		Request( const string& request );
		~Request() {}
		void	print() {
			cout << YELLOW "Method: " RESET << "[" << _req_line_.method << "]\n";
			cout << YELLOW "URI: " RESET << "[" << _req_line_.uri << "]\n";
			cout << YELLOW "HTTP version: " RESET << "[" << _req_line_.httpv << "]\n";
			map<string, string>::iterator it = headers.begin();
			for ( ; it != headers.end(); it++) {
				cout << YELLOW << it->first << ": " RESET << "[" << it->second << "]\n";
			}
		}
};
