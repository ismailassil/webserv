#include "Server.hpp"

Server::Server() : listen(-1), client_max_body_size(-1)
{
    server_names.clear();
    error_pages.clear();
    locations.clear();
}


void    Server::setListen(std::vector<std::string> values)
{
    std::string port;

    if (values.size() != 1)
        // throw std::runtime_error("Error: One 'Listen Port' in Server Required");
        {printf("Error\n"); exit(1);}
    port = values[0];
    for (size_t i = 0; i < port.size(); i++)
    {
        if (!std::isdigit(port[i]))
            // throw std::runtime_error("Error: Invalid 'Listen Port'");
            {printf("Error\n"); exit(1);}
    }
    listen = atoi(port.c_str());
}

void Server::setBodySize(std::vector<std::string> values)
{
    if (values.size() != 1)
        // throw std::runtime_error("Error: One 'Body Size' in Server Required");
        {printf("Error\n"); exit(1);}
    std::string size = values[0];
    size_t length = size.size();

    if (length == 0)
        // throw std::runtime_error("Error: Empty 'Body Size'");
        {printf("Error\n"); exit(1);}
    char suffix = size[length - 1];
    bool hasSuffix = (suffix == 'k' || suffix == 'K' || suffix == 'm' || suffix == 'M' || suffix == 'g' || suffix == 'G');
    for (size_t i = 0; i < length - (hasSuffix ? 1 : 0); i++)
    {
        if (!std::isdigit(size[i]))
            // throw std::runtime_error("Error: Invalid 'Body Size' format near '" + size + "'");
            {printf("Error\n"); exit(1);}
    }
    int numericValue = atoi(size.c_str());
    if (numericValue < 0)
        // throw std::runtime_error("Error: Negative 'Body Size' not allowed");
        {printf("Error\n"); exit(1);}
    if (hasSuffix)
    {
        switch (suffix)
        {
            case 'k':
            case 'K':
                numericValue *= 1024;
                break;
            case 'm':
            case 'M':
                numericValue *= 1024 * 1024;
                break;
            case 'g':
            case 'G':
                numericValue *= 1024 * 1024 * 1024;
                break;
            default:
                // throw std::runtime_error("Error: Unknown Unit in 'Body Size'");
                {printf("Error\n"); exit(1);}
        }
    }
    if (numericValue == 0)
        numericValue = -1;
    client_max_body_size = numericValue;
}

void    Server::setServerName(std::vector<std::string> values)
{
    for (size_t i = 0; i < values.size(); i++)
        server_names.push_back(values[i]);
}

void Server::setErrorPages(const std::vector<std::string> values)
{
    if (values.size() % 2 != 0)
        // throw std::runtime_error("Error: Mismatched error codes and pages in 'error_page' directive");
        {printf("Error\n"); exit(1);}
    for (size_t i = 0; i < values.size(); i += 2)
    {
        const std::string& errorCode = values[i];
        const std::string& errorPage = values[i + 1];
        for (size_t j = 0; j < errorCode.size(); j++)
        {
            if (!std::isdigit(errorCode[j]))
                // throw std::runtime_error("Error: Invalid error code '" + errorCode + "' in 'error_page' directive");
                {printf("Error\n"); exit(1);}
        }
        error_pages[atoi(errorCode.c_str())] = errorPage;
    }
}


void    Server::addLocation(Location location)
{
    locations.push_back(location);
}

void	Server::setDirectives(std::map<std::string, std::vector<std::string> > directives)
{
    for(std::map<std::string, std::vector<std::string> >::iterator iter = directives.begin(); iter!= directives.end(); iter++)
    {
        if (iter->first == "listen")
            setListen(iter->second);
        else if (iter->first == "client_max_body_size")
            setBodySize(iter->second);
        else if (iter->first == "server_name")
            setServerName(iter->second);
        else if (iter->first == "error_pages")
            setErrorPages(iter->second);
        else
            // throw std::runtime_error("Error: Invalid token '" + iter->first + "' in Server");
            {printf("Error\n"); exit(1);}
    }
}
