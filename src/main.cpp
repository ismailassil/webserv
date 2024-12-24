#include "ConfigParser.hpp"

int main()
{
    ConfigParser ConfigParser;
    ConfigParser.parseConfigFile("server.conf");
    return 0;
}
