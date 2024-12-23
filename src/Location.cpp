#include "Location.hpp"

Location::Location(): path(""), root(""), index(""), autoindex(false), cgi(""), upload_path(""), redirect("")     
{
    methods.clear();
}


Location::Location(std::string locationPath)
{
    path = locationPath;
}

void	Location::setSingDirectives(std::string key, std::vector<std::string> values)
{
    if (values.size() != 1)
        // throw std::runtime_error("Error: One '" + key + "' in 'Location' Required");
        {printf("Error\n"); exit(1);}
    std::string value = values[0];
    if (key == "root")
        root = value;
    else if (key == "index")
        index = value;
    else if (key == "autoindex")
    {
        if (value != "on" && value != "off")
            // throw std::runtime_error("Error: autoindex must have 'on' or 'off' value");
            {printf("Error\n"); exit(1);}
        autoindex = (value == "on");
    }
    else if (key == "upload_path")
        upload_path = value;
    else if (key == "redirect")
        redirect = value;
}

void	Location::setMethods(std::vector<std::string> values)
{
    std::string locationMethods[] = {"GET", "POST", "DELETE"};
    std::vector<std::string> validMethods(locationMethods, locationMethods + 3);
    for (size_t i = 0; i < values.size(); i++)
    {
        if (find(validMethods.begin(), validMethods.end(), values[i]) == validMethods.end())
            // throw std::runtime_error("Error: Invalid method '" + values[i] + "'");
            {printf("Error\n"); exit(1);}
        methods.insert(values[i]);
    }
}


void	Location::setDirectives(std::map<std::string, std::vector<std::string> > directives)
{
    std::string tokens[] = {"root", "index", "autoindex", "cgi", "upload_path", "redirect", "methods"};
    std::vector<std::string> locationTokens(tokens, tokens + 7);

    for(std::map<std::string, std::vector<std::string> >::iterator iter = directives.begin(); iter!= directives.end(); iter++)
    {
        if (iter->first == "methods")
            setMethods(iter->second);
        else if (find(locationTokens.begin(), locationTokens.end(), iter->first) != locationTokens.end())
            setSingDirectives(iter->first, iter->second);
        else
            // throw std::runtime_error("Error: Invalid token '" + iter->first + "' in Location");
            {printf("Error\n"); exit(1);}

    }
}