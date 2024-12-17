/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestBuilder.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:05:53 by iassil            #+#    #+#             */
/*   Updated: 2024/12/17 08:18:30 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Request.hpp"
#include <iostream> // IWYU pragma: keep
#include <map> // IWYU pragma: keep
#include <regex> // IWYU pragma: keep

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::map;
using std::istringstream;
using std::istream;
using std::getline;
using std::find;
using std::regex;

class RequestBuilder {
	private:
		string			rawRequestHeader;
		bool			isHeaderDone;
		req_stat		req_status;
		t_header_infos	header_info;
		Request*		req;
	
	public:
		RequestBuilder();
		~RequestBuilder();

		void	requestProcess( const string& comingRequest );
};