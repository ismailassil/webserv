#pragma once

#include "Server.hpp"

class ConfigParser {
    private:
        std::vector<Server> servers;
    public:
        ConfigParser();
		void    parseConfigFile(const std::string &filePath);
        void    tokenize(const std::string& configContent, std::vector<std::string>& tokens);
        void    parseTokens(const std::vector<std::string>& tokens);
        int     extractLocationContext(const std::vector<std::string>& tokens, int startIndex, Location& location);
        int     extractServerContext(const std::vector<std::string>& tokens, int startIndex, Server& server);
        bool    isValidPath(const std::string& path);
        void    checkConfigValidity();
        void    displayData();
};