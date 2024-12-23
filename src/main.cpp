#include "ConfigParser.hpp"

int main()
{
    ConfigParser ConfigParser;
    ConfigParser.parseConfigFile("server.conf");
    ConfigParser.displayData();
    return 0;
}
