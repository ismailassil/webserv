/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 10:44:55 by iassil            #+#    #+#             */
/*   Updated: 2024/12/23 18:37:27 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/RequestParser.hpp"

RequestParser::RequestParser() : isDone( false ) {}

const RequestLine& RequestParser::getRequestLine() const { return requestLine; }

const map< string, string >& RequestParser::getHeaders() const {
	return headers;
}

const string& RequestParser::getBody() const { return body; }

const bool& RequestParser::getStatus() const { return isDone; }
