#include "Config.hpp"

int main()
{
    Config config;
    config.parseConfigFile("server.conf");
    return 0;
}
