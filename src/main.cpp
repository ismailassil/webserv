/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 15:36:29 by iassil            #+#    #+#             */
/*   Updated: 2024/12/15 19:52:03 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../bits/Request.hpp"

int main( int ac, char **av ) {
	(void)ac, (void)av;
	std::string rawRequest = 
		"GET /index.html HTTP/1.1\r\n"
		"Host: www.example.com\r\n"
		"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/117.0.0.0 Safari/537.36\r\n"
		"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8\r\n"
		"Accept-Language: en-US,en;q=0.5\r\n"
		"Accept-Encoding: gzip, deflate, br\r\n"
		"Connection: keep-alive\r\n\r\n";

	Request* req;
	
	try {
		req = new Request(rawRequest);
	} catch ( const char * err ) {
		cerr << err;
		return 1;
	}
	
	req->print();
	
	return (0);
}