#pragma once

#include <iostream>
#include <vector>
#include <map>

class Location {
	private:
		std::string					path;
		std::string					root;
		std::string					index;
		std::vector<std::string>	methods;
		std::string					cgi;
		bool						autoindex;
		std::string					upload_path;
		std::string					redirect;
};