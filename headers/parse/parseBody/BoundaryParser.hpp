/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BoundaryParser.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 03:06:17 by iassil            #+#    #+#             */
/*   Updated: 2024/12/23 14:11:58 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../BodyParser.hpp" // IWYU pragma: keep
#include "../../bits.hpp"

class BoundaryParser : public BodyParser {
	private:
		void	parseFilenameBody( void );
		void	parseNameAttr( size_t& pos );
		void	parseFilenameAttr( size_t& pos, size_t& npos );
		void	parseNameBody( void );
		void	parseBoundaries( const istringstream& stream );
		void	parseBinary( const istringstream& stream );

	public:
		BoundaryParser() : BodyParser() {}
		
		void	parse( istringstream& );
};
