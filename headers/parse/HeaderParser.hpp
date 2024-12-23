/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeaderParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:29:17 by iassil            #+#    #+#             */
/*   Updated: 2024/12/18 09:28:23 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../RequestParser.hpp" // IWYU pragma: keep
#include "../bits.hpp"

class HeaderParser : public RequestParser {
	private:
		bool	isValidHeader( const string& );
		void	parseLine( const string& );
		bool	isDoubleCRLF( istream&, const string& );

	public:
		HeaderParser()
			: RequestParser() {}
		void	parse( istringstream& );
		void	print() const;
};