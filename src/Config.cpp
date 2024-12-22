#include "Config.hpp"

Config::Config() {};

const std::vector<Server>& Config::getServers() const
{
    return servers;
}

void Config::tokenize(const std::string& configContent, std::vector<std::string>& tokens)
{
    std::string currentToken;
    for (size_t i = 0; i < configContent.size(); i++)
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
                i++;
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
        throw std::runtime_error("Error: Can't Open Configuration File");
    while (std::getline(file, line))
        fileContent += line;
    tokenize(fileContent, tokens);
    parseTokens(tokens);
}

bool Config::isValidPath(const std::string& path)
{
    if (path.empty() || path[0] != '/')
        return false;
    for (size_t i = 0; i < path.size(); i++)
    {
        if (path[i] == ' ' || path[i] == '\t')
        {
            return false;
        }
    }

    return true;
}

int Config::extractLocationContext(const std::vector<std::string>& tokens, int startIndex, Location& location)
{
    std::map<std::string, std::vector<std::string> > directives;

    for (size_t i = startIndex; i < tokens.size(); i++) {
        if (tokens[i] == "}")
        {
            location.setDirectives(directives);
            return i;
        }
        else
        {
            std::string key = tokens[i];
            if (i + 1 < tokens.size())
            {
                i++;
                while (i < tokens.size() && tokens[i] != ";" && tokens[i] != "{" && tokens[i] != "}")
                {
                    directives[key].push_back(tokens[i]);
                    i++;
                }
                if (i >= tokens.size() || tokens[i] != ";")
                    throw std::runtime_error("Error: Missing ';' after values for key '" + key + "'");
            }

            else
            {
                printf("===== %s ====\n", key.c_str());
                throw std::runtime_error("Error: Invalid directive syntax near 1 '" + key + "'");
            }
        }
    }

    throw std::runtime_error("Error: Missing '}' to close 'location' context.");
}

int Config::extractServerContext(const std::vector<std::string>& tokens, int startIndex, Server& server)
{
    std::map<std::string, std::vector<std::string> > directives;

    for (size_t i = startIndex; i < tokens.size(); i++)
    {
        if (tokens[i] == "}")
        {
            server.setDirectives(directives);
            return i;
        }
        else if (tokens[i] == "location")
        {
            if (i + 3 < tokens.size() && tokens[i + 1] != "{" && tokens[i + 2] == "{")
            {
                std::string locationPath = tokens[i + 1];
                if (!isValidPath(locationPath))
                    throw std::runtime_error("Error: Invalid path for 'location': " + locationPath);
                Location location(locationPath);
                i = extractLocationContext(tokens, i + 3, location);
                server.addLocation(location);
            }
            else
                throw std::runtime_error("Error: Invalid syntax for 'location'");
        }
        else
        {
            std::string key = tokens[i];
            if (i + 1 < tokens.size())
            {
                i++;
                while (i < tokens.size() && tokens[i] != ";" && tokens[i] != "{" && tokens[i] != "}")
                {
                    directives[key].push_back(tokens[i]);
                    i++;
                }
                if (i >= tokens.size() || tokens[i] != ";")
                    throw std::runtime_error("Error: Missing ';' after values for key '" + key + "'");
            }

            else
            {
                printf("===== %s ====\n", key.c_str());
                throw std::runtime_error("Error: Invalid directive syntax near 2 '" + key + "'");
            }
        }
    }

    throw std::runtime_error("Error: Missing '}' to close 'server' context.");
}

void Config::parseTokens(const std::vector<std::string>& tokens)
{
    bool serverFound = false;

    for (size_t i = 0; i < tokens.size(); i++)
    {
        if (tokens[i] == "server")
        {
            Server currentServer;
            if (i + 1 < tokens.size() && tokens[i + 1] == "{")
            {
                serverFound = true;
                i = extractServerContext(tokens, i + 2, currentServer);
                servers.push_back(currentServer);
            }
            else
                throw std::runtime_error("Error: 'server' must be followed by '{'");
        }
        else
            throw std::runtime_error("Error: Unknown context");
    }

    if (!serverFound)
        throw std::runtime_error("Error: No 'server' context found in the configuration.");
}
