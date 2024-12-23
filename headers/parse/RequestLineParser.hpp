/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestLineParser.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:29:17 by iassil            #+#    #+#             */
/*   Updated: 2024/12/18 09:28:36 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../RequestParser.hpp" // IWYU pragma: keep
#include "../bits.hpp"

class RequestLineParser : public RequestParser {
	private:
		bool	isValidRequestLine( const string& );
		void	parseLine( const string& );
	
	public:
		RequestLineParser()
			: RequestParser() {}

		void	parse( istringstream& );
		void	print() const;
};