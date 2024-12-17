/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestBuilder.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:09:27 by iassil            #+#    #+#             */
/*   Updated: 2024/12/17 08:14:39 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../bits/RequestBuilder.hpp"

RequestBuilder::RequestBuilder() : isHeaderDone(false) {
	req = new Request();
}

RequestBuilder::~RequestBuilder() {
	delete req;
}

void	RequestBuilder::requestProcess( const string& comingRequest ) {
	size_t		pos;
	
	if (!isHeaderDone) {
		rawRequestHeader.append(comingRequest);

		pos = rawRequestHeader.find("\r\n\r\n");
		if ( pos != std::string::npos && !isHeaderDone ) {
			req->parseRequestHeader(rawRequestHeader);
			req_status = req->getRequestStatus();
			header_info = req->getHeaderInfos();
			rawRequestHeader.erase(pos + 4);
			isHeaderDone = true;
		}
		return ;
	}
	if ( !rawRequestHeader.empty() ) {
		req->parseRequestBody(rawRequestHeader, header_info);
	}
}

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