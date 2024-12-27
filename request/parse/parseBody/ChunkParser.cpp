/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChunkParser.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 03:06:21 by iassil            #+#    #+#             */
/*   Updated: 2024/12/27 20:54:40 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/parse/parseBody/ChunkParser.hpp"

bool ChunkParser::isEndBoundary( void ) {
	size_t pos = chunkInfo.requestChunk.find( "\r\n" + headerInfo.endBoundary );

	if ( pos != string::npos && pos == 0 ) {
		pushChunk();
		return true;
	}
	return false;
}

void ChunkParser::parseInnerBoundary( const string& body ) {
	size_t bd_pos = body.find( headerInfo.boundary );
	if ( bd_pos == string::npos || bd_pos != 0 )
		throw "-- BOUNDARY NOT FOUND --";

	size_t filenamePos = body.find( FILENAME );
	size_t namePos	   = body.find( NAME );
	size_t crPos	   = body.find( DCRNL, namePos );

	if ( filenamePos != string::npos && filenamePos < crPos ) {
		filenamePos += 12;
		namePos += 6;
		size_t epos			 = body.find( CRNL, filenamePos );
		size_t cpos			 = body.find( ";", namePos );
		string contentString = body.substr( filenamePos, epos - filenamePos );
		string contentStringName = body.substr( namePos, cpos - namePos );
		chunkInfo.filename		 = getAttr( contentString );
		chunkInfo.name			 = getAttr( contentStringName );
		outfile.open( "/Users/iassil/goinfre/_downloads/" + chunkInfo.filename,
					  ios::app | ios::binary );	 // to be removed
		if ( !outfile.is_open() )
			throw runtime_error( "-- failed to open - " + chunkInfo.filename +
								 " --" );

		size_t dcrPos = body.find( DCRNL );
		if ( dcrPos == string::npos )
			throw runtime_error( "-- DCRNL not found --" );
		bodyStatus.isFile = true;
	} else if ( namePos != string::npos ) {
		namePos += 6;
		size_t epos			 = body.find( CRNL, namePos );
		string contentString = body.substr( namePos, epos - namePos );
		chunkInfo.name		 = getAttr( contentString );

		size_t dcrPos = body.find( DCRNL );
		if ( dcrPos == string::npos )
			throw runtime_error( "-- DCRNL not found --" );
		bodyStatus.isText = true;
	} else
		throw BAD_REQUEST;
}

void ChunkParser::pushChunk( void ) {
	if ( bodyStatus.isFile ) {
		metaData.push_back( make_pair( chunkInfo.name, chunkInfo.filename ) );
		outfile.close();
		bodyStatus.isFile = false;
	} else if ( bodyStatus.isText ) {
		ofstream nOutfile( "/Users/iassil/goinfre/_downloads/" + chunkInfo.name,
						   ios::app | ios::binary );  // to be removed
		nOutfile.write( chunkInfo.Chunk.c_str(),
						chunkInfo.Chunk.length() );	 // to be removed
		nOutfile.close();							 // to be removed
		metaData.push_back( make_pair( chunkInfo.name, chunkInfo.Chunk ) ),
			chunkInfo.Chunk.clear();
		bodyStatus.isText = false;
	}
}

bool ChunkParser::parseHeadBody( void ) {
	if ( !bodyStatus.isheadLength ) {
		size_t pos = chunkInfo.requestChunk.find( CRNL );
		if ( pos == string::npos ) {
			// return true;
			throw runtime_error( "CRNL NOT FOUND - HEAD" );
		}
		const string& number = chunkInfo.requestChunk.substr( 0, pos );
		char*		  end;
		lengthInfo.chunkLength =
			static_cast< size_t >( strtol( number.c_str(), &end, 16 ) );
		chunkInfo.requestChunk.erase( 0, pos + 2 );
	}
	size_t currentLength =
		lengthInfo.chunkLength > chunkInfo.requestChunk.length()
			? chunkInfo.requestChunk.length()
			: lengthInfo.chunkLength;
	chunkInfo.BodyChunk.append(
		chunkInfo.requestChunk.substr( 0, currentLength ) );
	lengthInfo.chunkLength -= currentLength;
	bodyStatus.isheadLength = !lengthInfo.chunkLength;
	if ( lengthInfo.chunkLength == 0 ) {
		if ( isEndBoundary() ) return true;
		parseInnerBoundary( chunkInfo.BodyChunk );
		chunkInfo.BodyChunk.clear();
		bodyStatus.isheadLength = false;
		bodyStatus.isheadDone	= true;
		currentLength += 2;
	}
	chunkInfo.requestChunk.erase( 0, currentLength );
	return false;
}

bool ChunkParser::parseBodyLength( void ) {
	size_t i = 0;
	while ( i < chunkInfo.requestChunk.length() &&
			( chunkInfo.requestChunk[i] == '\n' ||
			  chunkInfo.requestChunk[i] == '\r' ) )
		i++;
	chunkInfo.requestChunk.erase( 0, i );
	size_t pos = chunkInfo.requestChunk.find( CRNL );
	if ( pos == string::npos ) {
		const string& number = chunkInfo.requestChunk.substr(
			0, chunkInfo.requestChunk.length() - 1 );
		char* end;
		lengthInfo.chunkLength =
			static_cast< size_t >( strtol( number.c_str(), &end, 16 ) );
		if ( *end == '\0' ) return true;
		throw runtime_error( "CRNL NOT FOUND - BODY -" );
	}
	const string& number = chunkInfo.requestChunk.substr( 0, pos );
	char*		  end;
	lengthInfo.chunkLength =
		static_cast< size_t >( strtol( number.c_str(), &end, 16 ) );
	chunkInfo.requestChunk.erase( 0, pos + 2 );
	bodyStatus.isbodySize = true;
	if ( lengthInfo.chunkLength == 2 &&
		 chunkInfo.requestChunk.substr( 0, 2 ) == "\r\n" ) {
		chunkInfo.requestChunk.erase( 0, 4 );
		bodyStatus.isbodySize = false;
		bodyStatus.isheadDone = false;
		pushChunk();
	}
	if ( isEndBoundary() ) return true;
	return false;
}

void ChunkParser::parseBodyChunk( void ) {
	size_t currentLength =
		lengthInfo.chunkLength > chunkInfo.requestChunk.length()
			? chunkInfo.requestChunk.length()
			: lengthInfo.chunkLength;
	lengthInfo.chunkLength -= currentLength;
	chunkInfo.BodyChunk.append(
		chunkInfo.requestChunk.substr( 0, currentLength ) );
	if ( lengthInfo.chunkLength == 0 ) {
		bodyStatus.isbodySize = false;
		if ( bodyStatus.isFile ) {
			outfile.write( chunkInfo.BodyChunk.c_str(),
						   chunkInfo.BodyChunk.length() );
			outfile.flush();
		} else if ( bodyStatus.isText ) {
			chunkInfo.Chunk.append( chunkInfo.BodyChunk );
		}
		chunkInfo.BodyChunk.clear();
		currentLength += 2;
	}
	chunkInfo.requestChunk.erase( 0, currentLength );
}

void ChunkParser::parseChunkedBoundaries( const istringstream& stream ) {
	chunkInfo.requestChunk.append( stream.str() );

	while ( !chunkInfo.requestChunk.empty() ) {
		if ( !bodyStatus.isheadDone ) {
			if ( parseHeadBody() ) return;
		}
		if ( bodyStatus.isheadDone && !chunkInfo.requestChunk.empty() ) {
			if ( !bodyStatus.isbodySize ) {
				if ( parseBodyLength() ) return;
			}
			if ( bodyStatus.isbodySize ) {
				parseBodyChunk();
			}
		}
	}
}
void ChunkParser::parseChunked( const istringstream& stream ) {
	chunkInfo.requestChunk.append( stream.str() );

	while ( !chunkInfo.requestChunk.empty() ) {
		if ( !bodyStatus.initDone ) {
			chunkInfo.filename = generateRandomName( headerInfo.contentType );
			outfile.open( chunkInfo.filename, ios::app | ios::binary );
			if ( !outfile.is_open() )
				throw runtime_error( "failed to open - " + chunkInfo.filename );
			bodyStatus.initDone = true;
		} else if ( bodyStatus.initDone ) {
			if ( !bodyStatus.perm ) {
				size_t pos = chunkInfo.requestChunk.find( CRNL );
				if ( pos == string::npos ) {
					const string& number = chunkInfo.requestChunk.substr(
						0, chunkInfo.requestChunk.length() - 1 );
					char* end;
					lengthInfo.chunkLength = static_cast< size_t >(
						strtol( number.c_str(), &end, 16 ) );
					if ( *end == '\0' ) return;
					throw runtime_error( "CRNL not found - Chunk Length" );
				}
				char* end;
				lengthInfo.chunkLength = static_cast< size_t >(
					strtol( chunkInfo.requestChunk.substr( 0, pos ).c_str(),
							&end, 16 ) );
				if ( lengthInfo.chunkLength == 0 ) {
					if ( outfile.is_open() ) outfile.close();
					return;
				}
				chunkInfo.requestChunk.erase( 0, pos + 2 );
				bodyStatus.perm = true;
			}
			if ( bodyStatus.perm ) {
				size_t sLength =
					lengthInfo.chunkLength > chunkInfo.requestChunk.length()
						? chunkInfo.requestChunk.length()
						: lengthInfo.chunkLength;
				lengthInfo.chunkLength -= sLength;
				chunkInfo.BodyChunk.append(
					chunkInfo.requestChunk.substr( 0, sLength ) );
				if ( !lengthInfo.chunkLength ) {
					bodyStatus.perm = false;
					chunkInfo.requestChunk.erase( 0, sLength + 2 );
					outfile.write( chunkInfo.BodyChunk.c_str(),
								   chunkInfo.BodyChunk.length() );
					outfile.flush();
					chunkInfo.BodyChunk.clear();
				} else
					chunkInfo.requestChunk.erase( 0, sLength );
			}
		}
	}
}

void ChunkParser::parse( istringstream& stream ) {
	if ( status == CHUNKED )
		parseChunked( stream );
	else if ( status == CHUNK_BOUND )
		parseChunkedBoundaries( stream );
}
