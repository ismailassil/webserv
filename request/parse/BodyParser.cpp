/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BodyParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 15:22:27 by iassil            #+#    #+#             */
/*   Updated: 2024/12/23 16:05:33 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/parse/BodyParser.hpp"

BodyParser::BodyParser() : RequestParser() {
	bodyStatus.perm= false;
	bodyStatus.initDone = false;
	bodyStatus.isheadDone = false;
	bodyStatus.isheadLength = false;
	bodyStatus.isbodySize = false;
	bodyStatus.isFile = false;
	bodyStatus.isText = false;
	lengthInfo.bodyLength = 0;
}

void	BodyParser::parse( istringstream& stream ) {
	if ( !isDone ) {
		if ( status == CHUNKED || status == CHUNK_BOUND ) {
			chunkParser = new ChunkParser();
			chunkParser->setStatus( status );
			chunkParser->setHeaderInfo( headerInfo );
		}
		else if ( status == BOUNDARIES || status == CONTENT_LENGTH || status == NO_CONTENT_LENGTH ) {
			boundaryParser = new BoundaryParser();
			boundaryParser->setStatus( status );
			boundaryParser->setHeaderInfo( headerInfo );
		}
		isDone = true;
	}
	switch (status) {
		case CHUNKED:
		case CHUNK_BOUND:
			chunkParser->parse( stream );
			break ;
		case BOUNDARIES:
		case CONTENT_LENGTH:
		case NO_CONTENT_LENGTH:
			boundaryParser->parse( stream );
			break ;
		case NONE:
			break;
	}
}

const string BodyParser::generateRandomName( const string& contentType ) {
	int num = rand() % 100 + 1;
	std::ostringstream oss;
	oss << num;
	return "_downloads/tmp" + oss.str() + contentType;
}

const string	BodyParser::getAttr( string& requestBody ) {
	size_t	pos = requestBody.find(";");
	if ( pos != string::npos ) {
		return requestBody.substr( 0, pos - 1 );
	}

	size_t	spos = requestBody.rfind("\"");
	return requestBody.substr( 0, spos );
}

void	BodyParser::print() const {
	cout << "============================" << endl;
	for ( vector<pair<string, string> >::const_iterator it = metaData.begin(); it != metaData.end(); it++ ) {
		cout << it->first << ": " << it->second << endl;
	}
	cout << "============================" << endl;
}

void	BodyParser::setHeaderInfo( HeaderInfo& headerInfo ) {
	this->headerInfo = headerInfo;
}

void	BodyParser::setStatus( RequestStatus& status_info ) {
	this->status = status_info;
}
