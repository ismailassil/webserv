/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BodyParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 15:22:27 by iassil            #+#    #+#             */
/*   Updated: 2024/12/19 21:57:50 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../bits/parse/BodyParser.hpp"

BodyParser::BodyParser() : RequestParser() {
	bodyStatus.perm= false;
	bodyStatus.initDone = false;
	bodyStatus.bodyDone = false;
	bodyStatus.isUp = false;
	bodyStatus.isNa = false;
	BodyLength = 0;
}

void	BodyParser::parse( istringstream& stream ) {
	switch (status) {
		case CHUNKED:
			parseChunckedBody( stream );
			break ;
		case CHUNK_BOUND:
		case BOUNDARIES:
			parseBoundaries( stream );
			break ;
		case CONTENT_LENGTH:
			parseBinary( stream );
			break ;
		case NO_CONTENT_LENGTH:
			parseBinary( stream );
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

void	BodyParser::parseBinary( const istringstream& stream ) {
	requestChunk.append(stream.str());

	while ( !requestChunk.empty() ) {
		cout << requestChunk << endl;
		if ( !bodyStatus.initDone ) {
			filename = generateRandomName( header_info.contentType );
			outfile.open( filename, std::ios::app | std::ios::binary );
			if ( !outfile.is_open() ) throw runtime_error("failed to open - " + filename );
			bodyStatus.initDone = true;
		} 
		if ( bodyStatus.initDone ) {
			outfile.write(requestChunk.c_str(), requestChunk.length());
			outfile.flush();

			if ( status == CONTENT_LENGTH ) {
				BodyLength += requestChunk.length();

				if ( BodyLength == header_info.contentLength ) {
					bodyStatus.initDone = false;
					if ( outfile.is_open() ) outfile.close();
				} else if ( BodyLength > header_info.contentLength ) {
					if ( outfile.is_open() ) outfile.close();
					remove(filename.c_str());
					throw BAD_REQUEST;
				}
			}
			requestChunk.clear();
		}
	}
}

void	BodyParser::parseFilenameBody( void ) {
	size_t	pos		= requestChunk.find( header_info.boundary );
	size_t	epos	= requestChunk.find( header_info.endBoundary );

	if ( pos == std::string::npos && epos == std::string::npos ) {
		outfile.write(requestChunk.c_str(), requestChunk.length());
		requestChunk.clear();
	}
	if ( pos != std::string::npos ) { // Boundary FOUND
		string	remaingStr = requestChunk.substr( 0, pos - 2 );
		requestChunk.erase( 0, pos );
		outfile.write(remaingStr.c_str(), remaingStr.length());
		metaData.push_back(make_pair(name, filename));
		bodyStatus.initDone = false;
		bodyStatus.isUp = false;
		if ( outfile.is_open() )
			outfile.close();
		outfile.close();
		filename.clear();
	}
	else if ( epos != std::string::npos ) { // endBoundary FOUND
		requestChunk.erase( epos - 2 );
		outfile.write(requestChunk.c_str(), requestChunk.length());
		metaData.push_back(make_pair(name, filename));
		if ( outfile.is_open() )
			outfile.close();
		requestChunk.clear();
		filename.clear();
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
		ofstream nOutfile( "_downloads/" + name , std::ios::app | std::ios::binary ); // to be removed
		nOutfile << chunk; // to be removed
		chunk.clear();

		bodyStatus.initDone = false;
		bodyStatus.isNa = false;
	}
	else if ( epos != std::string::npos ) { // endBoundary FOUND
		requestChunk.erase( epos - 2 );
		chunk.append(requestChunk);
		metaData.push_back(make_pair(name, chunk));
		ofstream nOutfile( "_downloads/" + name , std::ios::app | std::ios::binary ); // to be removed
		nOutfile << chunk; // to be removed
		chunk.clear();
		requestChunk.clear();
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

void	BodyParser::parseFilenameAttr( size_t& pos, size_t& npos ) {
	pos += 12;
	npos += 6;
	size_t	epos = requestChunk.find( CRNL, pos );
	size_t	cpos = requestChunk.find( ";", npos );
	string	contentString = requestChunk.substr( pos, epos - pos );
	string	contentStringName = requestChunk.substr( npos, cpos - npos );
	filename = getAttr( contentString );
	name = getAttr( contentStringName );
	outfile.open( "_downloads/" + filename , std::ios::app | std::ios::binary ); // to be removed
	if ( !outfile.is_open() ) throw runtime_error("failed to open - " + filename);

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

void	BodyParser::parseBoundaries( const istringstream& stream ) {
	requestChunk.append(stream.str());

	while ( !requestChunk.empty() ) {
		if ( !bodyStatus.initDone ) {
			size_t	bd_pos = requestChunk.find( header_info.boundary );
			if ( bd_pos == std::string::npos ) throw "BOUNDARY NOT FOUND";

			size_t	fl_pos = requestChunk.find( FILENAME );
			size_t	npos = requestChunk.find( NAME );
			size_t	crpos = requestChunk.find( DCRNL, npos );

			if ( fl_pos != std::string::npos && fl_pos < crpos ) {
				parseFilenameAttr( fl_pos, npos );
				bodyStatus.initDone = true;
				bodyStatus.isUp = true;
			} else if ( npos != std::string::npos ) {
				parseNameAttr( npos );
				bodyStatus.initDone = true;
				bodyStatus.isNa = true;
			}
			else throw BAD_REQUEST;
		} else {
			if ( bodyStatus.isUp )
				parseFilenameBody();
			else if ( bodyStatus.isNa )
				parseNameBody();
		}

	}
}

void	BodyParser::parseChunckedBody( const istringstream& stream ) {
	requestChunk.append(stream.str());

	while ( !requestChunk.empty() ) {
		
		if ( !bodyStatus.initDone ) {
			filename = generateRandomName( header_info.contentType );
			outfile.open( filename, std::ios::app | std::ios::binary );
			if ( !outfile.is_open() ) throw runtime_error("failed to open - " + filename );
			bodyStatus.initDone = true;
		} else if ( bodyStatus.initDone ) {
			if ( !bodyStatus.perm ) {
				// if ( bodyStatus.bodyDone ) {
				// 	size_t	pos = requestChunk.find( CRNL );
				// 	if ( pos == std::string::npos ) throw runtime_error( "CRNL not found" );
				// 	requestChunk.erase(0, pos + 2);
				// }
				size_t	pos = requestChunk.find( CRNL );
				char *end;
				chunkLength = static_cast<size_t>(strtol(requestChunk.substr(0, pos).c_str(), &end, 16));
				if ( chunkLength == 0 ) {
					if ( outfile.is_open() ) outfile.close();
					return ;
				}
				requestChunk.erase( 0, pos + 2 );
				bodyStatus.perm = true;
			}
			if ( bodyStatus.perm ) {
				// bodyStatus.bodyDone = true;
				size_t	sLength = chunkLength > requestChunk.length() ? requestChunk.length() : chunkLength;
				chunkLength -= sLength;
				MChunk += requestChunk.substr(0, sLength);
				if ( !chunkLength ) {
					bodyStatus.perm = false;
					requestChunk.erase(0, sLength + 2);
					outfile.write(MChunk.c_str(), MChunk.length());
					outfile.flush();
					MChunk.clear();
				} else requestChunk.erase(0, sLength);
			}
		}
	}
}

void	BodyParser::print() const {
	cout << "============================" << endl;
	for ( vector<pair<string, string> >::const_iterator it = metaData.begin(); it != metaData.end(); it++ ) {
		cout << it->first << ": " << it->second << endl;
	}
	cout << "============================" << endl;
}

void	BodyParser::setHeaderInfo( HeaderInfo& header_info ) {
	this->header_info = header_info;
}

void	BodyParser::setStatus( RequestStatus& status_info ) {
	this->status = status_info;
}
