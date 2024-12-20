#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include "Server.hpp"

class Config {
    private:
        std::vector<Server> servers;
    public:
        Config();
        void tokenize(const std::string& configContent, std::vector<std::string>& tokens);
		void parseConfigFile(const std::string &filePath);
		const std::vector<Server>& getServers() const;
};