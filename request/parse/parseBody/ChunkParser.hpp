/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChunkParser.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 03:07:16 by iassil            #+#    #+#             */
/*   Updated: 2025/01/30 17:37:34 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../../../srcs/bits.hpp"
#include "../BodyParser.hpp"  // IWYU pragma: keep

static int step;

class ChunkParser : public BodyParser {
	private:
		bool isEndBoundary( void );
		void parseInnerBoundary( const string& body );
		void pushChunk( void );
		bool parseHeadBody( void );
		bool parseBodyLength( void );
		void parseBodyChunk( void );
		void parseChunkedBoundaries( const istringstream& stream );
		void parseChunked( const istringstream& stream );

	public:
		ChunkParser() : BodyParser() {}
		~ChunkParser() {}

		void parse( istringstream& );
};
