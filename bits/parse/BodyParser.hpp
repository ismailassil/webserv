/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BodyParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:29:17 by iassil            #+#    #+#             */
/*   Updated: 2024/12/20 20:10:15 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../RequestParser.hpp" // IWYU pragma: keep
#include "../bits.hpp"

class BodyParser : public RequestParser {
	private:
		TaskStatus		bodyStatus;
		HeaderInfo		headerInfo;
		RequestStatus	status;
		ofstream		outfile;
		string			requestChunk;
		string			name;
		string			filename;
		string			chunk;
		string			MChunk;
		size_t			BodyLength;
		size_t			chunkLength;

		void	parseChunked( const istringstream& );
		void	parseChunkedBoundaries( const istringstream& );
		void	parseBoundaries( const istringstream& );
		void	parseBinary( const istringstream& );
		
		void	parseNameAttr( size_t& );
		void	parseFilenameAttr( size_t&, size_t& );

		void	parseFilenameBody( void );
		void	parseNameBody( void );

		void	parseInerBoundary( const string& body );
		
		const string	getAttr( string& );

		static const string generateRandomName( const string& );

	public:
		BodyParser();

		void	parse( istringstream& );
		
		void	setHeaderInfo( HeaderInfo& );
		void	setStatus( RequestStatus& );
		void	print() const;
};