#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <set>

#include "Server.hpp"

class Config {
    private:
        std::vector<Server> servers;
    public:
        Config();
		const std::vector<Server>& getServers() const;
		void parseConfigFile(const std::string &filePath);
        void tokenize(const std::string& configContent, std::vector<std::string>& tokens);
        void parseTokens(const std::vector<std::string>& tokens);
        void validateToken(const std::string& token, const std::string& context);
        int extractLocationContext(const std::vector<std::string>& tokens, int startIndex, Location& location);
        int extractServerContext(const std::vector<std::string>& tokens, int startIndex, Server& server);
};