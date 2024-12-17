/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 15:58:58 by iassil            #+#    #+#             */
/*   Updated: 2024/12/16 17:30:09 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../bits/Request.hpp"

Request::Request() {
	requestLineParser = new RequestLineParser();
	headerParser = new HeaderParser();
	bodyParser = new BodyParser();
}

Request::~Request() {
	vector<RequestParser*>::iterator it = request.begin();
	for (; it != request.end(); it++) {
		delete *it;
	}
}

void	Request::parseRequestHeader( const string& RawRequest ) {
	istringstream	stream(RawRequest);

	requestLineParser->parse(stream);
	headerParser->parse(stream);

	request.push_back(requestLineParser);
	request.push_back(headerParser);

	print();
}

void	Request::parseRequestBody( const string& RawRequest, t_header_infos& header_info ) {
	istringstream	stream(RawRequest);

	if (req_status == CHUNCKED)
		bodyParser->parseChunckedBody(stream, header_info);
}

req_stat	Request::getRequestStatus() {
	map<string, string>	requestHeader = request[1]->getHeaders();
	
	map<string, string>::iterator it_tr = requestHeader.find("transfer-encoding");
	map<string, string>::iterator it_ct = requestHeader.find("content-type");
	
	if ( it_tr == requestHeader.end() )
		throw REQUEST_IML;
	if ( it_tr != requestHeader.end() && it_tr->second.find("chuncked") != std::string::npos ) {
		if ( it_ct->second.find("multipart/form-data;") )
			return CHUNCK_BOUND;
		return CHUNCKED;
	}
	return NONE;
}

t_header_infos		Request::getHeaderInfos() {
	map<string, string>	requestHeader = request[1]->getHeaders();

	map<string, string>::iterator it_ct = requestHeader.find("content-type");
	map<string, string>::iterator it_cl = requestHeader.find("content-length");

	if ( it_ct != requestHeader.end() ) {
		string	type = it_ct->second;
		type.erase(0, type.find_first_of("/"));
		header_infos.content_type = type;
	}
	if ( it_ct != requestHeader.end() ) {
		header_infos.content_length = it_cl->second;
	}

	return header_infos;
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
void	Request::print() {
	vector<RequestParser*>::iterator it = request.begin();
	for (; it != request.end(); it++) {
		(*it)->print();
	}
}
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////