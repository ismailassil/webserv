/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BodyParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:29:17 by iassil            #+#    #+#             */
/*   Updated: 2024/12/18 14:54:37 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../RequestParser.hpp" // IWYU pragma: keep
#include "../bits.hpp"

class BodyParser : public RequestParser {
	private:
		TaskStatus		bodyStatus;
		HeaderInfo		header_info;
		RequestStatus	status;
		ofstream*		outfile;
		string			requestChunk;
		string			name;
		string			chunk;
		vector<pair<string, string> >	metaData;

		void	parseChunckedBody( istringstream& );
		void	parseBoundaries( istringstream& );
		
		void	parseNameAttr( size_t& );
		void	parseFilenameAttr( size_t& );

		void	parseFilenameBody( void );
		void	parseNameBody( void );
		
		const string	getAttr( string& );

	public:
		BodyParser();

		void	parse( istringstream& );
		
		void	setHeaderInfo( HeaderInfo& );
		void	setStatus( RequestStatus& );
		void	print() const;
};