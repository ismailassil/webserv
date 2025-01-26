/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:07:39 by iassil            #+#    #+#             */
/*   Updated: 2024/12/25 12:28:55 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "bits.hpp"

class RequestParser {
	protected:
		RequestLine						 requestLine;
		map< string, string >			 headers;
		string							 body;
		vector< pair< string, string > > metaData;
		bool							 isDone;

	public:
		RequestParser();
		virtual ~RequestParser() {}

		virtual void parse( istringstream& ) = 0;
		virtual void print() const			 = 0;

		const RequestLine&			 getRequestLine() const;
		const map< string, string >& getHeaders() const;
		const string&				 getBody() const;
		const bool&					 getStatus() const;

		virtual void setHeaderInfo( HeaderInfo& ) {}
		virtual void setStatus( RequestStatus& ) {}
};