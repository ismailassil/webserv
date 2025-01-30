/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestLineParser.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:29:17 by iassil            #+#    #+#             */
/*   Updated: 2025/01/30 17:37:49 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../RequestParser.hpp"	 // IWYU pragma: keep
#include "../../srcs/bits.hpp"

class RequestLineParser : public RequestParser {
	private:
		bool isValidRequestLine( const string& );
		void parseLine( const string& );

	public:
		RequestLineParser() : RequestParser() {}

		void		 parse( istringstream& );;
		void		 print() const;
};
