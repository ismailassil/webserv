/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestBuilder.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:09:27 by iassil            #+#    #+#             */
/*   Updated: 2024/12/19 20:15:51 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../bits/RequestBuilder.hpp"
#include <cstddef>

void	RequestBuilder::parseRequestHeader( const string& RawRequest ) {
	istringstream	stream(RawRequest);

	if ( !requestLineParser->getStatus() ) {
		requestLineParser->parse(stream);
	}
	if ( !headerParser->getStatus() ) {
		headerParser->parse(stream);
	}
}

void	RequestBuilder::parseRequestBody( string& RawRequest ) {
	istringstream	stream(RawRequest);

	if ( !isSettingDone ) {
		bodyParser->setHeaderInfo(headerInfo);
		bodyParser->setStatus(status);
		isSettingDone = true;
	}
	bodyParser->parse(stream);
}

void	RequestBuilder::getRequestStatus() {
	size_t boundary_start, boundary_end;
	map<string, string>	requestHeader = headerParser->getHeaders();
	
	map<string, string>::iterator it_tr = requestHeader.find("transfer-encoding");
	map<string, string>::iterator it_ct = requestHeader.find("content-type");
	map<string, string>::iterator it_cl = requestHeader.find("content-length");
	

	if ( it_tr != requestHeader.end() ) {
		if ( it_tr->second.find("chunked") != std::string::npos && it_ct != requestHeader.end() ) {
			if ( it_ct->second.find("boundary") != std::string::npos )
				status = CHUNK_BOUND;
			else status = CHUNKED;
		}
		return ;
	} else if ( it_ct != requestHeader.end() && ( boundary_start = it_ct->second.find("boundary") ) != std::string::npos ) {
		boundary_start += 9;
		boundary_end = it_ct->second.find("\r");

		if ( boundary_start != std::string::npos ) {
			setBoundary( it_ct, boundary_start, boundary_end );
			status = BOUNDARIES;
			return;
		}
	} else if ( it_ct != requestHeader.end() ) {
		if ( it_cl != requestHeader.end() )
			status = CONTENT_LENGTH;
		else status = NO_CONTENT_LENGTH;
	}
}

void		RequestBuilder::getHeaderInfos() {
	map<string, string>	requestHeader = headerParser->getHeaders();

	map<string, string>::iterator it_ct = requestHeader.find("content-type");
	map<string, string>::iterator it_cl = requestHeader.find("content-length");

	if ( it_ct != requestHeader.end() ) {
		string	type = it_ct->second;
		type.erase(0, type.find_first_of("/") + 1);
		if ( type == "plain" ) type = "txt";
		headerInfo.contentType = "." + type;
	}
	if ( it_ct != requestHeader.end() ) {
		char	*end;
		headerInfo.contentLength = static_cast<size_t>(strtol(it_cl->second.c_str(), &end, 10));
	}
}

void	RequestBuilder::setBoundary( map<string, string>::iterator& it, size_t& spos, size_t& epos ) {
	headerInfo.boundary.append("--");
	headerInfo.boundary.append(it->second.substr( spos, epos - spos + 1 ));
	headerInfo.endBoundary = headerInfo.boundary.substr( 0, headerInfo.boundary.length() ) + "--";
	headerInfo.boundary.append( CRNL );
	headerInfo.endBoundary.append( CRNL );
}

void	RequestBuilder::build(const string& incomingRequest) {
	size_t			pos;
	
	rawRequest.append(incomingRequest);
	if ( !isHeaderDone ) {

		pos = rawRequest.find("\r\n\r\n");
		if ( pos != std::string::npos ) {
			parseRequestHeader(rawRequest);
			getRequestStatus(), getHeaderInfos();
			rawRequest.erase(0, pos + 4);
			isHeaderDone = true;
		}
		return ;
	}
	if ( !rawRequest.empty() ) {
		parseRequestBody(rawRequest);
		rawRequest.clear();
	}
}

void	RequestBuilder::print() const {
	cout << GREEN"============REQUEST LINE============" << RESET << endl;
	requestLineParser->print();
	cout << GREEN"==========REQUEST HEADER============" << RESET << endl;
	headerParser->print();
	cout << GREEN"================BODY===============" << RESET << endl;
	bodyParser->print();
}

RequestBuilder::RequestBuilder()
	: requestLineParser(new RequestLineParser()),
	headerParser(new HeaderParser()),
	bodyParser(new BodyParser()),
	status(NONE),
	isHeaderDone(false),
	isSettingDone(false) {}

/*
BODY - Handle
	-> Transfer-encoding
		-> chunked (vanilla)
			-> chunked
			-> chunked with boundaries
		-> otherwise: 501 Not Implemented
	-> boundaries
	-> Content-length
*/