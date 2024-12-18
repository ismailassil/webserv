/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BodyParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:29:17 by iassil            #+#    #+#             */
/*   Updated: 2024/12/18 09:29:46 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../RequestParser.hpp" // IWYU pragma: keep
#include "../bits.hpp"

class BodyParser : public RequestParser {
	private:
		bool			hasPermission;
		bool			isBodyTopDone;
		bool			isBodyDone;
		HeaderInfo		header_info;
		RequestStatus	status;
		ofstream*		outfile;
		string			requestChunk;

		void	parseChunckedBody( istringstream& );
		void	parseBoundaries( istringstream& );
		
		const string	getFilname( string& );

	public:
		BodyParser()
			: RequestParser(),
			hasPermission(false),
			isBodyTopDone(false),
			isBodyDone(false) {}

		void	parse( istringstream& );
		
		void	setHeaderInfo( HeaderInfo& );
		void	setStatus( RequestStatus& );
		void	print() const;
};