/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BoundaryParser.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 03:06:17 by iassil            #+#    #+#             */
/*   Updated: 2025/03/17 21:01:38 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../../../srcs/bits.hpp"
#include "../BodyParser.hpp"

class BoundaryParser : public BodyParser {
	private:
		void parseFilenameBody( void );
		void parseNameAttr( size_t& pos );
		void parseFilenameAttr( size_t& pos, size_t& npos );
		void parseNameBody( void );
		void parseBoundaries( const istringstream& stream );
		void parseBinary( const istringstream& stream );

	public:
		BoundaryParser();
		~BoundaryParser();

		void parse( istringstream& );
};
