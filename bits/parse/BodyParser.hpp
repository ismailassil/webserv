/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BodyParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:29:17 by iassil            #+#    #+#             */
/*   Updated: 2024/12/16 18:54:57 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../RequestParser.hpp" // IWYU pragma: keep
#include <iostream> // IWYU pragma: keep
#include <map> // IWYU pragma: keep
#include <sstream> // IWYU pragma: keep
#include <cstring> // IWYU pragma: keep
#include <string> // IWYU pragma: keep
#include <regex> // IWYU pragma: keep
#include <algorithm> // IWYU pragma: keep

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

#define REQUEST_IML "501 Not Implemented\n"
#define BAD_REQUEST "400 Bad Request\n"

typedef struct s_header_infos {
	string	content_type;
	string	content_length;
}	t_header_infos;

class BodyParser : public RequestParser {
	private:
		bool	isChuncked = false;
	public:
		void	parse( istringstream& stream );
		void	parseChunckedBody( istringstream& stream, t_header_infos& header_info );
		void	print();
};