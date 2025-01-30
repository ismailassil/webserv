/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 10:44:55 by iassil            #+#    #+#             */
/*   Updated: 2025/01/30 17:39:33 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestParser.hpp"

RequestParser::RequestParser() : isDone( false ) {}

const RequestLine& RequestParser::getRequestLine() const { return requestLine; }

const map< string, string >& RequestParser::getHeaders() const {
	return headers;
}

const string& RequestParser::getBody() const { return body; }

const bool& RequestParser::getStatus() const { return isDone; }
