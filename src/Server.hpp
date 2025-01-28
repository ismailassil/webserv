#pragma once

#include "Location.hpp"

class Server {
	// private:
	public:
		int							listen;
		std::vector<std::string>	server_names;
		std::map<int, std::string>	error_pages;
		int							client_max_body_size;
		std::vector<Location>		locations;
	public:
		Server();
		void	setDirectives(std::map<std::string, std::vector<std::string> > directives);
		void	setListen(std::vector<std::string> values);
		void	setBodySize(std::vector<std::string> values);
		void	setServerName(std::vector<std::string> values);
		void	setErrorPages(std::vector<std::string> values);
		void	addLocation(Location location);

		// Getters
		int							getListen() const;
		std::vector<std::string>	getServerNames() const;
		std::map<int, std::string>	getErrorPages() const;
		int							getClientMaxBodySize() const;
		std::vector<Location>		getLocations() const;
};