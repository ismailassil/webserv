/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestBuilder.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:09:27 by iassil            #+#    #+#             */
/*   Updated: 2024/12/18 10:27:16 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../bits/RequestBuilder.hpp"

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
	map<string, string>	requestHeader = headerParser->getHeaders();
	
	map<string, string>::iterator it_tr = requestHeader.find("transfer-encoding");
	map<string, string>::iterator it_ct = requestHeader.find("content-type");
	
	if ( it_tr != requestHeader.end() && it_ct != requestHeader.end() )
		throw BAD_REQUEST;
	if ( it_tr != requestHeader.end() && it_tr->second.find("chunked") != std::string::npos ) { // CHUNKED + BOUNDARY
		if ( it_ct->second.find("multipart/form-data;") ) { // CHUNKED + BOUNDARY
			status = CHUNK_BOUND;
			return ;
		}
		status = CHUNKED;
	}
	if ( it_ct != requestHeader.end() ) {
		size_t boundary_start = it_ct->second.find("boundary") + 9;
		size_t boundary_end = it_ct->second.find("\r");

		if (boundary_start != std::string::npos) { // BOUNDARY
			headerInfo.boundary.append("--");
			headerInfo.boundary.append(it_ct->second.substr( boundary_start, boundary_end - boundary_start + 1 ));
			headerInfo.endBoundary = headerInfo.boundary.substr( 0, headerInfo.boundary.length() ) + "--";
			headerInfo.boundary.append( CRNL );
			headerInfo.endBoundary.append( CRNL );
			status = BOUNDARIES;
			return;
		}
	}
}

void		RequestBuilder::getHeaderInfos() {
	map<string, string>	requestHeader = headerParser->getHeaders();

	map<string, string>::iterator it_ct = requestHeader.find("content-type");
	map<string, string>::iterator it_cl = requestHeader.find("content-length");

	if ( it_ct != requestHeader.end() ) {
		string	type = it_ct->second;
		type.erase(0, type.find_first_of("/"));
		headerInfo.contentType = type;
	}
	if ( it_ct != requestHeader.end() ) {
		headerInfo.contentLength = it_cl->second;
	}
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
	cout << GREEN"===========REQUEST LINE============" << RESET << endl;
	requestLineParser->print();
	cout << GREEN"=========REQUEST HEADER============" << RESET << endl;
	headerParser->print();
	cout << GREEN"===================================" << RESET << endl;
	// bodyParser->print();
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