#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

#include "Location.hpp"

class Server {
	private:
		int							listen;
		std::vector<std::string>	server_names;
		std::map<int, std::string>	error_pages;
		int							client_max_body_size;
		std::vector<Location>		locations;
	public:
		void	setDirectives(std::map<std::string, std::vector<std::string> > directives);
		void	setListen(std::string key, std::vector<std::string> values);
		void	setServerName(std::string key, std::vector<std::string> values);
		void	addLocation(Location location);
		void	clear_server();
};