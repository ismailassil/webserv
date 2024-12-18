/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bits.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 09:22:40 by iassil            #+#    #+#             */
/*   Updated: 2024/12/17 11:10:10 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream> // IWYU pragma: keep
#include <map> // IWYU pragma: keep
#include <regex> // IWYU pragma: keep
#include <iostream> // IWYU pragma: keep
#include <map> // IWYU pragma: keep
#include <sstream> // IWYU pragma: keep
#include <cstring> // IWYU pragma: keep
#include <string> // IWYU pragma: keep
#include <regex> // IWYU pragma: keep
#include <algorithm> // IWYU pragma: keep
#include <sstream> // IWYU pragma: keep
#include <fstream> // IWYU pragma: keep
#include <utility> // IWYU pragma: keep
#include <cstddef> // IWYU pragma: keep


#define RED		"\x1b[31m"
#define GREEN	"\x1b[1;32m"
#define YELLOW	"\x1b[33m"
#define BLUE	"\x1b[34m"
#define MAGENTA	"\x1b[35m"
#define CYAN	"\x1b[36m"
#define RESET	"\x1b[0m"

#define REQUEST_IML 	"501 Not Implemented\n"
#define BAD_REQUEST 	"400 Bad Request\n"
#define	HTTP_VERSION	"505 HTTP Version Not Supported"
#define HOST			"host"

#define HTTP_V			"HTTP/1.1"
#define GET				"GET"
#define POST			"POST"
#define DELETE			"DELETE"
#define C_DISP			"Content-Disposition:"
#define FILENAME		"; filename=\""
#define NAME			"name=\""
#define QUOTE			"\""
#define CRNL			"\r\n"
#define DCRNL			"\r\n\r\n"

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
using std::ofstream;
using std::ios;
using std::runtime_error;
using std::pair;
using std::make_pair;
using std::vector;

struct RequestLine {
	string	method;
	string	uri;
	string	httpVersion;
};

struct TaskStatus {
	bool	perm;
	bool	initDone;
	bool	bodyDone;
	bool	isUp;
	bool	isNa;
};

struct HeaderInfo {
	string	contentType;
	string	contentLength;
	string	boundary;
	string	endBoundary;
};

enum RequestStatus {
	CHUNKED,
	CHUNK_BOUND,
	BOUNDARIES,
	CONTENT_LENGTH,
	NONE
};
