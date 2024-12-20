#pragma once

#include <iostream>
#include <vector>
#include <map>

#include "Location.hpp"

class Server {
	private:
		int							listen;
		std::vector<std::string>	server_names;
		std::map<int, std::string>	error_pages;
		int							client_max_body_size;
		std::vector<Location>		locations;
};