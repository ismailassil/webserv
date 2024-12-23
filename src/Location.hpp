#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <set>
#include <algorithm>
#include <stdlib.h>

class Location {
	// private:
	public:
		std::string					path;
		std::string					root;
		std::string					index;
		bool						autoindex;
		std::string					cgi;
		std::string					upload_path;
		std::string					redirect;
		std::set<std::string>		methods;
	public:
		Location();
		Location(std::string locationPath);
		void	setDirectives(std::map<std::string, std::vector<std::string> > directives);
		void	setSingDirectives(std::string key, std::vector<std::string> values);
		void	setMethods(std::vector<std::string> values);
};