/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BodyParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 15:22:27 by iassil            #+#    #+#             */
/*   Updated: 2024/12/18 11:39:16 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../bits/parse/BodyParser.hpp"
#include <fstream>

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

const string	BodyParser::getFilname( string& requestBody ) {
	size_t	pos = requestBody.find(";");
	if ( pos != std::string::npos ) {
		return requestBody.substr( 0, pos - 2 );
	}

	size_t	spos = requestBody.rfind("\"");
	return requestBody.substr( 0, spos );
}

void	BodyParser::parseBoundaries( istringstream& stream ) {
	requestChunk.append(stream.str());
	if ( !isBodyTopDone ) {
		size_t	bd_pos = requestChunk.find( header_info.boundary );
		if ( bd_pos == std::string::npos ) throw "BOUNDARY NOT FOUND";

		size_t	fl_pos = requestChunk.find( FILENAME );
		if ( fl_pos == std::string::npos ) throw "FILENAME NOT FOUND";

		fl_pos += 12;
		size_t	epos = requestChunk.find( CRNL, fl_pos );
		string	contentString = requestChunk.substr( fl_pos, epos - fl_pos );
		const string	filename = getFilname( contentString );
		outfile = new ofstream( "__" + filename + ".py", std::ios::app | std::ios::binary );
		if ( !outfile->is_open() ) delete outfile, throw std::runtime_error("Failed to open - " + filename);

		size_t	pos = requestChunk.find( DCRNL );
		
		size_t	endpos = pos + 4;
		// cout << YELLOW << "[" << requestChunk.substr(0, endpos) << "]" << RESET << endl;
		requestChunk.erase( 0, endpos );
		
		isBodyTopDone = true;
	}
	if ( isBodyTopDone ) {
		size_t	epos	= requestChunk.find( header_info.endBoundary );
		size_t	pos		= requestChunk.find( header_info.boundary );
		
		if ( pos == std::string::npos && epos == std::string::npos ) {
			ofstream a("FILE", std::ios::app | std::ios::binary);
			
			// cout << "--------------------------------------------" << endl;
			cout << "[" << (int)requestChunk[0] << "]";
			a << "\n--------------------------------------------" << endl;
			outfile->write(requestChunk.c_str(), requestChunk.length());
			requestChunk.clear();
		}
		if ( epos != std::string::npos ) {
			requestChunk.erase( epos - 2 );
			outfile->write(requestChunk.c_str(), requestChunk.length());
			if ( outfile ) {
				outfile->close();
				delete outfile;
			}
			return ;
		}
		if ( pos != std::string::npos ) {
			string	remaingStr = requestChunk.substr( 0, pos - 2 );
			requestChunk.erase( 0, pos );
			outfile->write(remaingStr.c_str(), remaingStr.length());
			isBodyTopDone = false;
			outfile->close();
			delete outfile;
		}
	}
}

void	BodyParser::parseChunckedBody( istringstream& stream ) {
	(void)stream;
	(void)hasPermission;
	(void)outfile;
	(void)isBodyDone;
	(void)isBodyTopDone;
}

void	BodyParser::print() const {
	
}

void	BodyParser::setHeaderInfo( HeaderInfo& header_info ) {
	this->header_info = header_info;
}

void	BodyParser::setStatus( RequestStatus& status_info ) {
	this->status = status_info;
}
