/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestBuilder.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:09:27 by iassil            #+#    #+#             */
/*   Updated: 2024/12/27 15:54:39 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/RequestBuilder.hpp"

void RequestBuilder::parseRequestHeader( const string& RawRequest ) {
	istringstream stream( RawRequest );

	if ( !requestParser[REQUEST_LINE]->getStatus() ) {
		requestParser[REQUEST_LINE]->parse( stream );
	}
	if ( !requestParser[HEADER]->getStatus() ) {
		requestParser[HEADER]->parse( stream );
	}
}

void RequestBuilder::parseRequestBody( string& RawRequest ) {
	istringstream stream( RawRequest );

	if ( !isSettingDone ) {
		requestParser[BODY]->setHeaderInfo( headerInfo );
		requestParser[BODY]->setStatus( status );
		isSettingDone = true;
	}
	requestParser[BODY]->parse( stream );
}

void RequestBuilder::getRequestStatus() {
	size_t				  boundary_start, boundary_end;
	map< string, string > requestHeader = requestParser[HEADER]->getHeaders();

	map< string, string >::iterator it_tr =
		requestHeader.find( "transfer-encoding" );
	map< string, string >::iterator it_ct =
		requestHeader.find( "content-type" );
	map< string, string >::iterator it_cl =
		requestHeader.find( "content-length" );

	if ( it_tr != requestHeader.end() ) {
		if ( it_tr->second.find( "chunked" ) != string::npos &&
			 it_ct != requestHeader.end() &&
			 it_ct->second.find( "boundary" ) != string::npos &&
			 ( boundary_start = it_ct->second.find( "boundary" ) ) !=
				 string::npos ) {
			boundary_start += 9;
			boundary_end = it_ct->second.find( "\r" );
			setBoundary( it_ct, boundary_start, boundary_end );
			status = CHUNK_BOUND;
		} else if ( it_ct != requestHeader.end() )
			status = CHUNKED;
		return;
	} else if ( it_ct != requestHeader.end() &&
				( boundary_start = it_ct->second.find( "boundary" ) ) !=
					string::npos ) {
		boundary_start += 9;
		boundary_end = it_ct->second.find( "\r" );

		if ( boundary_start != string::npos ) {
			setBoundary( it_ct, boundary_start, boundary_end );
			status = BOUNDARIES;
			return;
		}
	} else if ( it_ct != requestHeader.end() ) {
		if ( it_cl != requestHeader.end() )
			status = CONTENT_LENGTH;
		else
			status = NO_CONTENT_LENGTH;
	}
}

void RequestBuilder::getHeaderInfos() {
	map< string, string > requestHeader = requestParser[HEADER]->getHeaders();

	map< string, string >::iterator it_ct =
		requestHeader.find( "content-type" );
	map< string, string >::iterator it_cl =
		requestHeader.find( "content-length" );

	if ( it_ct != requestHeader.end() ) {
		string type = it_ct->second;
		type.erase( 0, type.find_first_of( "/" ) + 1 );
		if ( type == "plain" )
			type = ".txt";
		else if ( type == "octet-stream" )
			type = "";
		else
			type = "." + type;
		headerInfo.contentType = type;
	}
	if ( it_ct != requestHeader.end() ) {
		char* end;
		headerInfo.contentLength =
			static_cast< size_t >( strtol( it_cl->second.c_str(), &end, 10 ) );
	}
}

void RequestBuilder::setBoundary( map< string, string >::iterator& it,
								  size_t& spos, size_t& epos ) {
	headerInfo.boundary.append( "--" );
	headerInfo.boundary.append( it->second.substr( spos, epos - spos + 1 ) );
	headerInfo.endBoundary =
		headerInfo.boundary.substr( 0, headerInfo.boundary.length() ) + "--";
	headerInfo.boundary.append( CRNL );
	headerInfo.endBoundary.append( CRNL );
}

void RequestBuilder::build( const string& incomingRequest ) {
	size_t pos;

	rawRequest.append( incomingRequest );
	if ( !isHeaderDone ) {
		pos = rawRequest.find( "\r\n\r\n" );
		if ( pos != string::npos ) {
			parseRequestHeader( rawRequest );
			getRequestStatus(), getHeaderInfos();
			rawRequest.erase( 0, pos + 4 );
			isHeaderDone = true;
		}
	}
	if ( requestParser[REQUEST_LINE]->getRequestLine().method == POST &&
		 isHeaderDone && !rawRequest.empty() ) {
		parseRequestBody( rawRequest );
		rawRequest.clear();
	}
}

void RequestBuilder::print() const {
	cout << GREEN "============REQUEST_LINE============" << RESET << endl;
	requestParser[REQUEST_LINE]->print();
	cout << GREEN "===========REQUEST_HEADER===========" << RESET << endl;
	requestParser[HEADER]->print();
	cout << GREEN "================BODY================" << RESET << endl;
	requestParser[BODY]->print();
	cout << GREEN "====================================" << RESET << endl;
}

RequestBuilder::RequestBuilder() {
	requestParser[REQUEST_LINE] = new RequestLineParser();
	requestParser[HEADER]		= new HeaderParser();
	requestParser[BODY]			= new BodyParser();
	status						= NONE;
	isHeaderDone				= false;
	isSettingDone				= false;
}

RequestBuilder::~RequestBuilder() {
	for ( int i = 0; i < 3; i++ ) {
		if ( requestParser[i] ) {
			delete requestParser[i];
			requestParser[i] = NULL;
		}
	}
}
