/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 10:44:55 by iassil            #+#    #+#             */
/*   Updated: 2024/12/16 11:18:55 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../bits/RequestParser.hpp"

struct requestLine&	RequestParser::getRequestLine() {
	return requestLine;
}

map<string, string>& RequestParser::getHeaders() {
	return headers;
}

string&				RequestParser::getBody() {
	return body;
}