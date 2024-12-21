#include "Server.hpp"

void    Server::setListen(std::string key, std::vector<std::string> values)
{
    int         i;
    std::string port;

    for (i = 0; i < values.size() - 1; i++)
        ;
    port = values[i];
    for (size_t j = 0; j < port.size(); ++j)
    {
        if (!std::isdigit(port[j]))
            throw std::runtime_error("Error: Invalid Listen Port");
    }
    listen = atoi(port.c_str());
}

void    Server::setServerName(std::string key, std::vector<std::string> values)
{
    for (int i = 0; i < values.size(); i++)
        server_names.push_back(values[i]);
}

void	Server::setDirectives(std::map<std::string, std::vector<std::string> > directives)
{
    for(std::map<std::string, std::vector<std::string> >::iterator iter = directives.begin(); iter!= directives.end(); iter++)
    {
        if (iter->first == "listen")
            setListen(iter->first, iter->second);
        if (iter->first == "server_name")
            setServerName(iter->first, iter->second);
        else
            throw std::runtime_error("Error: Invalid token '" + iter->first + "' in Server");
    }
}
