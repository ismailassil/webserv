#include "ConfigParser.hpp"

ConfigParser::ConfigParser() {};

void ConfigParser::tokenize(const std::string& configContent, std::vector<std::string>& tokens)
{
    std::string currentToken;
    for (size_t i = 0; i < configContent.size(); i++)
    {
        char c = configContent[i];
        if (c == '#')
        {
            while (i < configContent.size() && configContent[i] != '\n')
                i++;
        }
        else if (c == '{' || c == '}' || c == ';')
        {
            if (!currentToken.empty()) {
                tokens.push_back(currentToken);
                currentToken.clear();
            }
            tokens.push_back(std::string(1, c));
            if (c == ';')
                while (i < configContent.size() && configContent[i] == ';')
                    i++;
        }
        else if (isspace(c) || c == '\n')
        {
            if (!currentToken.empty())
            {
                tokens.push_back(currentToken);
                currentToken.clear();
            }
            // while (i < configContent.size() && isspace(configContent[i]))
            //     i++;
        }
        else
            currentToken += c;
    }
    if (!currentToken.empty()) {
        tokens.push_back(currentToken);
    }
}

bool ConfigParser::isValidPath(const std::string& path)
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

int ConfigParser::extractLocationContext(const std::vector<std::string>& tokens, int startIndex, Location& location)
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
                size_t j = i;
                while (i < tokens.size() && tokens[i] != ";" && tokens[i] != "{" && tokens[i] != "}")
                {
                    directives[key].push_back(tokens[i]);
                    i++;
                }
                if (j == i || i >= tokens.size() || tokens[i] != ";")
                    // throw std::runtime_error("Error: Missing ';' after values for key '" + key + "'");
                    {printf("Error\n"); exit(1);}
            }

            else
                // throw std::runtime_error("Error: Invalid directive syntax near 1 '" + key + "'");
                {printf("Error\n"); exit(1);}
        }
    }

    // throw std::runtime_error("Error: Missing '}' to close 'location' context.");
    {printf("Error\n"); exit(1);}
}

int ConfigParser::extractServerContext(const std::vector<std::string>& tokens, int startIndex, Server& server)
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
                    {printf("Error\n"); exit(1);}
                    // throw std::runtime_error("Error: Invalid path for 'location': " + locationPath);
                Location location(locationPath);
                i = extractLocationContext(tokens, i + 3, location);
                server.addLocation(location);
            }
            else
                // throw std::runtime_error("Error: Invalid syntax for 'location'");
                {printf("Error\n"); exit(1);}
        }
        else
        {
            std::string key = tokens[i];
            if (i + 1 < tokens.size())
            {
                i++;
                size_t j = i;
                while (i < tokens.size() && tokens[i] != ";" && tokens[i] != "{" && tokens[i] != "}")
                {
                    directives[key].push_back(tokens[i]);
                    i++;
                }
                if (j == i || i >= tokens.size() || tokens[i] != ";")
                    // throw std::runtime_error("Error: Missing ';' after values for key '" + key + "'");
                    {printf("Error\n"); exit(1);}
            }

            else
                // throw std::runtime_error("Error: Invalid directive syntax near 2 '" + key + "'");
                {printf("Error\n"); exit(1);}
        }
    }
    // throw std::runtime_error("Error: Missing '}' to close 'server' context.");
    {printf("Error\n"); exit(1);}
}

void ConfigParser::parseTokens(const std::vector<std::string>& tokens, std::vector<Server>& servers)
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
                // throw std::runtime_error("Error: 'server' must be followed by '{'");
                {printf("Error\n"); exit(1);}
        }
        else
            // throw std::runtime_error("Error: Unknown context");
            {printf("Error\n"); exit(1);}
    }

    if (!serverFound)
        // throw std::runtime_error("Error: No 'server' context found in the configuration.");
        {printf("Error\n"); exit(1);}
}

void ConfigParser::checkConfigValidity(std::vector<Server>& servers) {
    std::set<int> serverPorts;

    for (std::vector<Server>::iterator serverIt = servers.begin(); serverIt != servers.end(); ++serverIt) {
        if (serverIt->listen == -1) {
            printf("Warning: 'listen' directive missing. Defaulting to port 80.\n");
            serverIt->listen = 80;
        } else if (serverIt->listen < 1 || serverIt->listen > 65535) {
            printf("Error: Invalid port '%d' in 'listen' directive. Must be between 1 and 65535.\n", serverIt->listen);
            exit(1);
        }

        if (serverPorts.find(serverIt->listen) != serverPorts.end()) {
            printf("Error: Duplicate 'listen' port '%d' found in configuration.\n", serverIt->listen);
            exit(1);
        }
        serverPorts.insert(serverIt->listen);

        if (serverIt->server_names.empty()) {
            printf("Warning: No 'server_name' specified. Defaulting to 'default_server'.\n");
            serverIt->server_names.push_back("default_server");
        }
        if (serverIt->locations.empty()) {
            printf("Error: No 'location' context found in the configuration for server listening on port '%d'.\n", serverIt->listen);
            exit(1);
        }

        std::set<std::string> locationPaths;
        for (std::vector<Location>::iterator locIt = serverIt->locations.begin(); locIt != serverIt->locations.end(); ++locIt) {
            if (locIt->path.empty()) {
                printf("Error: 'location' context has an empty path in server listening on port '%d'.\n", serverIt->listen);
                exit(1);
            }
            if (locationPaths.find(locIt->path) != locationPaths.end()) {
                printf("Error: Duplicate 'location' path '%s' found in server listening on port '%d'.\n", locIt->path.c_str(), serverIt->listen);
                exit(1);
            }
            locationPaths.insert(locIt->path);
        }
    }
}

void ConfigParser::parseConfigFile(const std::string &filePath, std::vector<Server>& servers)
{
    std::ifstream file(filePath.c_str());
    if (!file.is_open())
        // throw std::runtime_error("Error: Can't Open Configuration File");
        {printf("Error\n"); exit(1);}
    std::string line;
    std::string fileContent;
    std::vector<std::string> tokens;

    while (std::getline(file, line))
    {
        fileContent += line;
        if (!file.eof())
            fileContent += '\n';
    }
    tokenize(fileContent, tokens);
    parseTokens(tokens, servers);
    checkConfigValidity(servers);
}