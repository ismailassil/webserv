/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BodyParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 15:22:27 by iassil            #+#    #+#             */
/*   Updated: 2024/12/18 16:35:56 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../bits/parse/BodyParser.hpp"

BodyParser::BodyParser() : RequestParser() {
	bodyStatus.perm= false;
	bodyStatus.initDone = false;
	bodyStatus.bodyDone = false;
	bodyStatus.isUp = false;
	bodyStatus.isNa = false;
}

void	BodyParser::parse( istringstream& stream ) {
	switch (status) {

        case CHUNKED:
			parseChunckedBody( stream );
        case CHUNK_BOUND:
        case BOUNDARIES:
			parseBoundaries( stream );
        case CONTENT_LENGTH:
        case NONE:
          break;
	}
}

void	BodyParser::parseFilenameBody( void ) {
	size_t	pos		= requestChunk.find( header_info.boundary );
	size_t	epos	= requestChunk.find( header_info.endBoundary );

	if ( pos == std::string::npos && epos == std::string::npos ) {
		outfile->write(requestChunk.c_str(), requestChunk.length());
		requestChunk.clear();
	}
	if ( pos != std::string::npos ) { // Boundary FOUND
		string	remaingStr = requestChunk.substr( 0, pos - 2 );
		requestChunk.erase( 0, pos );
		outfile->write(remaingStr.c_str(), remaingStr.length());
		bodyStatus.initDone = false;
		bodyStatus.isUp = false;
		outfile->close();
		delete outfile;
	}
	else if ( epos != std::string::npos ) { // endBoundary FOUND
		requestChunk.erase( epos - 2 );
		outfile->write(requestChunk.c_str(), requestChunk.length());
		if ( outfile ) {
			outfile->close();
			delete outfile;
		}
		requestChunk.clear();

		return ;
	}
}

void	BodyParser::parseNameBody( void ) {
	size_t	pos		= requestChunk.find( header_info.boundary );
	size_t	epos	= requestChunk.find( header_info.endBoundary );
	
	if ( pos == std::string::npos && epos == std::string::npos ) {
		chunk.append(requestChunk);
		requestChunk.clear();
	}
	if ( pos != std::string::npos ) { // Boundary FOUND
		string	remaingStr = requestChunk.substr( 0, pos - 2 );
		requestChunk.erase( 0, pos );
		chunk.append(remaingStr);
		metaData.push_back(make_pair(name, chunk));
		ofstream nOutfile( "_downloads/" + name , std::ios::app | std::ios::binary );
		nOutfile << chunk;
		chunk.clear();

		bodyStatus.initDone = false;
		bodyStatus.isNa = false;
	}
	else if ( epos != std::string::npos ) { // endBoundary FOUND
		requestChunk.erase( epos - 2 );
		chunk.append(requestChunk);
		metaData.push_back(make_pair(name, chunk));
		ofstream nOutfile( "_downloads/" + name , std::ios::app | std::ios::binary );
		nOutfile << chunk;
		chunk.clear();
		requestChunk.clear();

		return ;
	}
}

void	BodyParser::parseNameAttr( size_t& pos ) {
	pos += 6;
	size_t	epos = requestChunk.find( CRNL, pos );
	string	contentString = requestChunk.substr( pos, epos - pos );
	name = getAttr( contentString );
	
	size_t	dpos = requestChunk.find( DCRNL );
	
	size_t	endpos = dpos + 4;
	requestChunk.erase( 0, endpos );
}

void	BodyParser::parseFilenameAttr( size_t& pos ) {
	pos += 12;
	size_t	epos = requestChunk.find( CRNL, pos );
	string	contentString = requestChunk.substr( pos, epos - pos );
	const string&	filename = getAttr( contentString );
	outfile = new ofstream( "_downloads/" + filename , std::ios::app | std::ios::binary );
	if ( !outfile->is_open() ) delete outfile, throw std::runtime_error("Failed to open - " + filename);

	size_t	dpos = requestChunk.find( DCRNL );
	
	size_t	endpos = dpos + 4;
	requestChunk.erase( 0, endpos );
}

const string	BodyParser::getAttr( string& requestBody ) {
	size_t	pos = requestBody.find(";");
	if ( pos != std::string::npos ) {
		return requestBody.substr( 0, pos - 1 );
	}

	size_t	spos = requestBody.rfind("\"");
	return requestBody.substr( 0, spos );
}

void	BodyParser::parseBoundaries( istringstream& stream ) {
	requestChunk.append(stream.str());

	while ( !requestChunk.empty() ) {
		if ( !bodyStatus.initDone ) {
			size_t	bd_pos = requestChunk.find( header_info.boundary );
			if ( bd_pos == std::string::npos ) throw "BOUNDARY NOT FOUND";

			size_t	fl_pos = requestChunk.find( FILENAME );
			size_t	npos = requestChunk.find( NAME );
			size_t	crpos = requestChunk.find( DCRNL, npos );

			if ( fl_pos != std::string::npos && fl_pos < crpos ) {
				parseFilenameAttr( fl_pos );
				bodyStatus.initDone = true;
				bodyStatus.isUp = true;
			} else if ( npos != std::string::npos ) {
				parseNameAttr( npos );
				bodyStatus.initDone = true;
				bodyStatus.isNa = true;
			}
			else throw BAD_REQUEST;
		}
		if ( bodyStatus.initDone && bodyStatus.isUp ) {
			parseFilenameBody();
		} else if ( bodyStatus.initDone && bodyStatus.isNa ) {
			parseNameBody();
		}

	}
}

void	BodyParser::parseChunckedBody( istringstream& stream ) {
	(void)stream;
	(void)outfile;
	(void)bodyStatus.perm;
	(void)bodyStatus.bodyDone;
	(void)bodyStatus.initDone;
	(void)bodyStatus.isNa;
	(void)bodyStatus.isUp;
}

void	BodyParser::print() const {
	
}

void	BodyParser::setHeaderInfo( HeaderInfo& header_info ) {
	this->header_info = header_info;
}

void	BodyParser::setStatus( RequestStatus& status_info ) {
	this->status = status_info;
}
