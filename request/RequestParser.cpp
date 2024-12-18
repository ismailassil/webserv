/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 10:44:55 by iassil            #+#    #+#             */
/*   Updated: 2024/12/17 12:39:58 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../bits/RequestParser.hpp"

const RequestLine&	RequestParser::getRequestLine() const
{
	return requestLine;
}

const map<string, string>&	RequestParser::getHeaders() const
{
	return headers;
}

const string&	RequestParser::getBody() const
{
	return body;
}


const bool&		RequestParser::getStatus() const {
	return isDone;
}
