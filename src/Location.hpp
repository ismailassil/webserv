#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <set>

class Location {
	private:
		std::string					path;
		std::string					root;
		std::string					index;
		std::set<std::string>		methods;
		std::string					cgi;
		bool						autoindex;
		std::string					upload_path;
		std::string					redirect;
	public:
		Location();
		Location(std::string locationPath);
		void	setDirectives(std::map<std::string, std::vector<std::string> > directives);
		void	setSingDirectives(std::string key, std::vector<std::string> values);
		void	setMethods(std::vector<std::string> values);
};