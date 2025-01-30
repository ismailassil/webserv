/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BodyParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:29:17 by iassil            #+#    #+#             */
/*   Updated: 2025/01/30 19:10:38 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../../srcs/bits.hpp"
#include "../RequestParser.hpp"	 // IWYU pragma: keep

class BodyParser : public RequestParser {
	protected:
		BodyStatus	  bodyStatus;
		HeaderInfo	  headerInfo;
		RequestStatus status;
		ChunkInfo	  chunkInfo;
		LengthInfo	  lengthInfo;
		ofstream	  outfile;

		BodyParser* bodyParser;

		static const string generateRandomName( const string& );
		static const string getAttr( string& );

	public:
		BodyParser();
		virtual ~BodyParser();

		void parse( istringstream& );

		void setHeaderInfo( HeaderInfo& );
		void setStatus( RequestStatus& );
		void print() const;
};

#include "parseBody/BoundaryParser.hpp"	 // IWYU pragma: keep
#include "parseBody/ChunkParser.hpp"	 // IWYU pragma: keep
