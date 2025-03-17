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

#include <netinet/in.h>	 // IWYU pragma: keep
#include <sys/socket.h>	 // IWYU pragma: keep
#include <unistd.h>		 // IWYU pragma: keep

#include <algorithm>  // IWYU pragma: keep
#include <cstddef>	  // IWYU pragma: keep
#include <cstring>	  // IWYU pragma: keep
#include <exception>
#include <sstream>	 // IWYU pragma: keep
#include <fstream>	 // IWYU pragma: keep
#include <ios>		 // IWYU pragma: keep
#include <iostream>	 // IWYU pragma: keep
#include <map>		 // IWYU pragma: keep
// #include <regex>	 // IWYU pragma: keep
#include <set>		 // IWYU pragma: keep
#include <sstream>	 // IWYU pragma: keep
#include <string>	 // IWYU pragma: keep
#include <utility>	 // IWYU pragma: keep
#include <vector>	 // IWYU pragma: keep

#define RED		"\x1b[31m"
#define GREEN	"\x1b[1;32m"
#define YELLOW	"\x1b[1;33m"
#define BLUE	"\x1b[1;34m"
#define MAGENTA "\x1b[1;35m"
#define CYAN	"\x1b[1;36m"
#define RESET	"\x1b[0m"

#define REQUEST_IML	 "501 Not Implemented\n"
#define BAD_REQUEST	 "400 Bad Request\n"
#define HTTP_VERSION "505 HTTP Version Not Supported"
#define HOST		 "host"

#define HTTP_V	 "HTTP/1.1"
#define GET		 "GET"
#define POST	 "POST"
#define DELETE	 "DELETE"
#define C_DISP	 "Content-Disposition:"
#define FILENAME "; filename=\""
#define NAME	 "name=\""
#define QUOTE	 "\""
#define CRNL	 "\r\n"
#define DCRNL	 "\r\n\r\n"

#define BUFFER_SIZE 8192

using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::find;
using std::getline;
using std::ifstream;
using std::ios;
using std::istream;
using std::istringstream;
using std::make_pair;
using std::map;
using std::ofstream;
using std::pair;
// using std::regex;
using std::remove;
using std::runtime_error;
using std::set;
using std::string;
using std::strtol;
using std::vector;
using std::stringstream;

struct RequestLine {
		string method;
		string uri;
		string httpVersion;
};

struct BodyStatus {
		bool perm;
		bool initDone;
		bool isheadDone;
		bool isheadLength;
		bool isbodySize;
		bool isFile;
		bool isText;
};

struct HeaderInfo {
		string contentType;
		size_t contentLength;
		string boundary;
		string endBoundary;
};

enum RequestStatus {
	CHUNKED,
	CHUNK_BOUND,
	BOUNDARIES,
	CONTENT_LENGTH,
	NO_CONTENT_LENGTH,
	NONE
};

enum ParserIndex { REQUEST_LINE = 0,
				   HEADER		= 1,
				   BODY			= 2 };

struct ChunkInfo {
		string requestChunk;
		string name;
		string filename;
		string Chunk;
		string BodyChunk;
};

struct LengthInfo {
		size_t bodyLength;
		size_t chunkLength;
};

string intToString( int num );
