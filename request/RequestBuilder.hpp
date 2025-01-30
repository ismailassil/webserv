/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestBuilder.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:05:53 by iassil            #+#    #+#             */
/*   Updated: 2025/01/30 17:36:17 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "RequestParser.hpp"
#include "../srcs/bits.hpp"
#include "parse/BodyParser.hpp"			// IWYU pragma: keep
#include "parse/HeaderParser.hpp"		// IWYU pragma: keep
#include "parse/RequestLineParser.hpp"	// IWYU pragma: keep

class BodyParser;
class ChunkParser;
class BoundaryParser;

class RequestBuilder {
	private:
		string		   rawRequest;
		RequestParser* requestParser[3];

		RequestStatus status;
		HeaderInfo	  headerInfo;
		bool		  isHeaderDone;
		bool		  isSettingDone;

	public:
		RequestBuilder();
		~RequestBuilder();

		void build( const string& );
		void print() const;

		void parseRequestHeader( const string& );
		void parseRequestBody( string& );

		void getRequestStatus();
		void getHeaderInfos();

		void setBoundary( map< string, string >::iterator&, size_t&, size_t& );
};