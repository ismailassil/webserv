/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BoundaryParser.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 03:04:10 by iassil            #+#    #+#             */
/*   Updated: 2024/12/27 15:17:10 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/parse/parseBody/BoundaryParser.hpp"

void BoundaryParser::parseFilenameBody( void ) {
	size_t pos	= chunkInfo.requestChunk.find( headerInfo.boundary );
	size_t epos = chunkInfo.requestChunk.find( headerInfo.endBoundary );

	if ( pos == string::npos && epos == string::npos ) {
		outfile.write( chunkInfo.requestChunk.c_str(),
					   chunkInfo.requestChunk.length() );
		chunkInfo.requestChunk.clear();
	}
	if ( pos != string::npos ) {  // Boundary FOUND
		string remaingStr = chunkInfo.requestChunk.substr( 0, pos - 2 );
		chunkInfo.requestChunk.erase( 0, pos );
		outfile.write( remaingStr.c_str(), remaingStr.length() );
		metaData.push_back( make_pair( chunkInfo.name, chunkInfo.filename ) );
		bodyStatus.initDone = false;
		bodyStatus.isFile	= false;
		if ( outfile.is_open() ) outfile.close();
		outfile.close();
		chunkInfo.filename.clear();
	} else if ( epos != string::npos ) {  // endBoundary FOUND
		chunkInfo.requestChunk.erase( epos - 2 );
		outfile.write( chunkInfo.requestChunk.c_str(),
					   chunkInfo.requestChunk.length() );
		metaData.push_back( make_pair( chunkInfo.name, chunkInfo.filename ) );
		if ( outfile.is_open() ) outfile.close();
		chunkInfo.requestChunk.clear();
		chunkInfo.filename.clear();
	}
}

void BoundaryParser::parseNameBody( void ) {
	size_t pos	= chunkInfo.requestChunk.find( headerInfo.boundary );
	size_t epos = chunkInfo.requestChunk.find( headerInfo.endBoundary );

	if ( pos == string::npos && epos == string::npos ) {
		chunkInfo.Chunk.append( chunkInfo.requestChunk );
		chunkInfo.requestChunk.clear();
	}
	if ( pos != string::npos ) {  // Boundary FOUND
		string remaingStr = chunkInfo.requestChunk.substr( 0, pos - 2 );
		chunkInfo.requestChunk.erase( 0, pos );
		chunkInfo.Chunk.append( remaingStr );
		metaData.push_back( make_pair( chunkInfo.name, chunkInfo.Chunk ) );
		ofstream nOutfile( "/Users/iassil/goinfre/_downloads/" + chunkInfo.name,
						   ios::app | ios::binary );  // to be removed
		nOutfile << chunkInfo.Chunk;				  // to be removed
		nOutfile.close();							  // to be removed
		chunkInfo.Chunk.clear();

		bodyStatus.initDone = false;
		bodyStatus.isText	= false;
	} else if ( epos != string::npos ) {  // endBoundary FOUND
		chunkInfo.requestChunk.erase( epos - 2 );
		chunkInfo.Chunk.append( chunkInfo.requestChunk );
		metaData.push_back( make_pair( chunkInfo.name, chunkInfo.Chunk ) );
		ofstream nOutfile( "/Users/iassil/goinfre/_downloads/" + chunkInfo.name,
						   ios::app | ios::binary );  // to be removed
		nOutfile << chunkInfo.Chunk;				  // to be removed
		nOutfile.close();							  // to be removed
		chunkInfo.Chunk.clear();
		chunkInfo.requestChunk.clear();
	}
}

void BoundaryParser::parseNameAttr( size_t& pos ) {
	pos += 6;
	size_t epos			 = chunkInfo.requestChunk.find( CRNL, pos );
	string contentString = chunkInfo.requestChunk.substr( pos, epos - pos );
	chunkInfo.name		 = getAttr( contentString );

	size_t dpos = chunkInfo.requestChunk.find( DCRNL );

	size_t endpos = dpos + 4;
	chunkInfo.requestChunk.erase( 0, endpos );
}

void BoundaryParser::parseFilenameAttr( size_t& pos, size_t& npos ) {
	pos += 12;
	npos += 6;
	size_t epos			 = chunkInfo.requestChunk.find( CRNL, pos );
	size_t cpos			 = chunkInfo.requestChunk.find( ";", npos );
	string contentString = chunkInfo.requestChunk.substr( pos, epos - pos );
	string contentStringName =
		chunkInfo.requestChunk.substr( npos, cpos - npos );
	chunkInfo.filename = getAttr( contentString );
	chunkInfo.name	   = getAttr( contentStringName );
	outfile.open( "/Users/iassil/goinfre/_downloads/" + chunkInfo.filename,
				  ios::app | ios::binary );	 // to be removed
	if ( !outfile.is_open() )
		throw runtime_error( "failed to open - " + chunkInfo.filename );

	size_t dpos = chunkInfo.requestChunk.find( DCRNL );

	size_t endpos = dpos + 4;
	chunkInfo.requestChunk.erase( 0, endpos );
}

void BoundaryParser::parseBoundaries( const istringstream& stream ) {
	chunkInfo.requestChunk.append( stream.str() );

	while ( !chunkInfo.requestChunk.empty() ) {
		if ( !bodyStatus.initDone ) {
			size_t bd_pos = chunkInfo.requestChunk.find( headerInfo.boundary );
			if ( bd_pos == string::npos ) throw "BOUNDARY NOT FOUND";

			size_t fl_pos = chunkInfo.requestChunk.find( FILENAME );
			size_t npos	  = chunkInfo.requestChunk.find( NAME );
			size_t crpos  = chunkInfo.requestChunk.find( DCRNL, npos );

			if ( fl_pos != string::npos && fl_pos < crpos ) {
				parseFilenameAttr( fl_pos, npos );
				bodyStatus.initDone = true;
				bodyStatus.isFile	= true;
			} else if ( npos != string::npos ) {
				parseNameAttr( npos );
				bodyStatus.initDone = true;
				bodyStatus.isText	= true;
			} else
				throw BAD_REQUEST;
		}
		if ( bodyStatus.initDone ) {
			if ( bodyStatus.isFile )
				parseFilenameBody();
			else if ( bodyStatus.isText )
				parseNameBody();
		}
	}
}

void BoundaryParser::parseBinary( const istringstream& stream ) {
	chunkInfo.requestChunk.append( stream.str() );

	while ( !chunkInfo.requestChunk.empty() ) {
		if ( !bodyStatus.initDone ) {
			chunkInfo.filename = generateRandomName( headerInfo.contentType );
			outfile.open( chunkInfo.filename, ios::app | ios::binary );
			if ( !outfile.is_open() )
				throw runtime_error( "failed to open - " + chunkInfo.filename );
			bodyStatus.initDone = true;
		}
		if ( bodyStatus.initDone ) {
			outfile.write( chunkInfo.requestChunk.c_str(),
						   chunkInfo.requestChunk.length() );
			outfile.flush();

			if ( status == CONTENT_LENGTH ) {
				lengthInfo.bodyLength += chunkInfo.requestChunk.length();

				if ( lengthInfo.bodyLength == headerInfo.contentLength ) {
					bodyStatus.initDone = false;
					if ( outfile.is_open() ) outfile.close();
				} else if ( lengthInfo.bodyLength > headerInfo.contentLength ) {
					if ( outfile.is_open() ) outfile.close();
					remove( chunkInfo.filename.c_str() );
					throw BAD_REQUEST;
				}
			}
			chunkInfo.requestChunk.clear();
		}
	}
}

void BoundaryParser::parse( istringstream& stream ) {
	if ( status == BOUNDARIES )
		parseBoundaries( stream );
	else if ( status == CONTENT_LENGTH || status == NO_CONTENT_LENGTH )
		parseBinary( stream );
}
