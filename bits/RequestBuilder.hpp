/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestBuilder.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:05:53 by iassil            #+#    #+#             */
/*   Updated: 2024/12/19 19:28:13 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "parse/BodyParser.hpp"
#include "parse/HeaderParser.hpp"
#include "parse/RequestLineParser.hpp"
#include "bits.hpp"

class RequestBuilder {
	private:
		string				rawRequest;
		
		RequestLineParser*	requestLineParser;
		HeaderParser*		headerParser;
		BodyParser*			bodyParser;
		
		RequestStatus		status;
		HeaderInfo			headerInfo;
		bool				isHeaderDone;
		bool				isSettingDone;

	public:
		RequestBuilder();

		void	build( const string& );
		void	print() const;

		void	parseRequestHeader( const string& );
		void	parseRequestBody( string& );

		void	getRequestStatus();
		void	getHeaderInfos();

		void	setBoundary( map<string, string>::iterator&, size_t&, size_t& );
};