#pragma once

#include "Server.hpp"

class ConfigParser {
    public:
        ConfigParser();
		void    parseConfigFile(const std::string &filePath, std::vector<Server>& servers);
        void    tokenize(const std::string& configContent, std::vector<std::string>& tokens);
        void    parseTokens(const std::vector<std::string>& tokens, std::vector<Server>& servers);
        int     extractLocationContext(const std::vector<std::string>& tokens, int startIndex, Location& location);
        int     extractServerContext(const std::vector<std::string>& tokens, int startIndex, Server& server);
        bool    isValidPath(const std::string& path);
        void    checkConfigValidity(std::vector<Server>& servers);
};