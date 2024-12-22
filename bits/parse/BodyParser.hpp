/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BodyParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:29:17 by iassil            #+#    #+#             */
/*   Updated: 2024/12/22 07:28:33 by iassil           ###   ########.fr       */
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

		bool	parseHeadBody( void );
		bool	parseBodyLength( void );
		void	parseBodyChunk( void );
		void	parseInnerBoundary( const string& body );
		void	pushChunk( void );
		bool	isEndBoundary( void );
		
		static const string generateRandomName( const string& );
		const string	getAttr( string& );

	public:
		BodyParser();

		void	parse( istringstream& );
		
		void	setHeaderInfo( HeaderInfo& );
		void	setStatus( RequestStatus& );
		void	print() const;
};