#include "Config.hpp"

Config::Config() {};

const std::vector<Server>& Config::getServers() const
{
    return servers;
}

void Config::tokenize(const std::string& configContent, std::vector<std::string>& tokens)
{
    std::string currentToken;
    for (size_t i = 0; i < configContent.size(); ++i)
    {
        char c = configContent[i];
        if (c == '{' || c == '}' || c == ';')
        {
            if (!currentToken.empty()) {
                tokens.push_back(currentToken);
                currentToken.clear();
            }
            tokens.push_back(std::string(1, c));
        }
        else if (isspace(c))
        {
            if (!currentToken.empty())
            {
                tokens.push_back(currentToken);
                currentToken.clear();
            }
        }
        else if (c == '#')
        {
            while (i < configContent.size() && configContent[i] != '\n') {
                ++i;
            }
        }
        else
            currentToken += c;
    }
    if (!currentToken.empty()) {
        tokens.push_back(currentToken);
    }
}

void Config::parseConfigFile(const std::string &filePath)
{
    std::ifstream file(filePath.c_str());
    std::string line;
    std::string fileContent;
    std::vector<std::string> tokens;

    if (!file.is_open())
        throw ("Error\n"); // to edit
    while (std::getline(file, line))
        fileContent += line;
    tokenize(fileContent, tokens);
    std::vector<std::string> it;
    for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); ++it)
        printf("token: %s\n", it->c_str());
}


