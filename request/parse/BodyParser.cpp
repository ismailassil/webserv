/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BodyParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 15:22:27 by iassil            #+#    #+#             */
/*   Updated: 2025/01/30 19:10:17 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BodyParser.hpp"

BodyParser::BodyParser() : RequestParser() {
	bodyParser				= NULL;
	bodyStatus.perm			= false;
	bodyStatus.initDone		= false;
	bodyStatus.isheadDone	= false;
	bodyStatus.isheadLength = false;
	bodyStatus.isbodySize	= false;
	bodyStatus.isFile		= false;
	bodyStatus.isText		= false;
	lengthInfo.bodyLength	= 0;
}
BodyParser::~BodyParser() {
	if ( bodyParser ) {
		delete bodyParser;
		bodyParser = NULL;
	}
}

void BodyParser::parse( istringstream& stream ) {
	if ( !isDone ) {
		if ( status == CHUNKED || status == CHUNK_BOUND )
			bodyParser = new ChunkParser();
		else if ( status == BOUNDARIES || status == CONTENT_LENGTH ||
				  status == NO_CONTENT_LENGTH )
			bodyParser = new BoundaryParser();
		else
			throw "PARSE NOT FOUND";
		bodyParser->setStatus( status );
		bodyParser->setHeaderInfo( headerInfo );
		isDone = true;
	}
	bodyParser->parse( stream );
}

const string BodyParser::generateRandomName( const string& contentType ) {
	int				   num = rand() % 100 + 1;
	std::ostringstream oss;
	oss << num;
	return "/Users/iassil/goinfre/_downloads/tmp" + oss.str() + contentType;
}

const string BodyParser::getAttr( string& requestBody ) {
	size_t pos = requestBody.find( ";" );
	if ( pos != string::npos ) {
		return requestBody.substr( 0, pos - 1 );
	}

	size_t spos = requestBody.rfind( "\"" );
	return requestBody.substr( 0, spos );
}

void BodyParser::print() const {
	for ( vector< pair< string, string > >::const_iterator it =
			  metaData.begin();
		  it != metaData.end(); it++ ) {
		cout << it->first << ": " << it->second << endl;
	}
}

void BodyParser::setHeaderInfo( HeaderInfo& headerInfo ) {
	this->headerInfo = headerInfo;
}

void BodyParser::setStatus( RequestStatus& status_info ) {
	this->status = status_info;
}
