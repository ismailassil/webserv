#include "WebServer.hpp"

WebServer::WebServer() {}

void WebServer::initialize(const std::string& configFile) {
    configParser.parseConfigFile(configFile, servers);
    configParser.checkConfigValidity(servers);
}


void WebServer::run() {
}


void WebServer::shutdown() {
    // Close the server
    // Close all connections
}

void WebServer::displayData() {
    int serverCount = 1;

    for (std::vector<Server>::const_iterator serverIt = servers.begin(); serverIt != servers.end(); ++serverIt, ++serverCount) {
        std::cout << "Server " << serverCount << ":\n";

        std::cout << "  Listen: " << serverIt->listen << "\n";
        std::cout << "  Server Names: ";
        for (std::vector<std::string>::const_iterator nameIt = serverIt->server_names.begin(); nameIt != serverIt->server_names.end(); ++nameIt) {
            if (nameIt != serverIt->server_names.begin()) std::cout << ", ";
            std::cout << *nameIt;
        }
        std::cout << "\n";

        std::cout << "  Client Max Body Size: ";
        if (serverIt->client_max_body_size > 0)
            std::cout << serverIt->client_max_body_size << " bytes\n";
        else
            std::cout << "Unlimited\n";

        std::cout << "  Error Pages:\n";
        for (std::map<int, std::string>::const_iterator errorIt = serverIt->error_pages.begin(); errorIt != serverIt->error_pages.end(); ++errorIt) {
            std::cout << "    " << errorIt->first << " -> " << errorIt->second << "\n";
        }

        std::cout << "  Locations:\n";
        int locationCount = 1;
        for (std::vector<Location>::const_iterator locIt = serverIt->locations.begin(); locIt != serverIt->locations.end(); ++locIt, ++locationCount) {
            std::cout << "    Location " << locationCount << ":\n";

            std::cout << "      Path: " << locIt->path << "\n";
            std::cout << "      Root: " << locIt->root << "\n";
            std::cout << "      Index: " << locIt->index << "\n";
            std::cout << "      Autoindex: " << (locIt->autoindex ? "on" : "off") << "\n";
            std::cout << "      Upload Path: " << locIt->upload_path << "\n";
            std::cout << "      Redirect: " << locIt->redirect << "\n";

            std::cout << "      Methods: ";
            for (std::set<std::string>::const_iterator methodIt = locIt->methods.begin(); methodIt != locIt->methods.end(); ++methodIt) {
                if (methodIt != locIt->methods.begin()) std::cout << ", ";
                std::cout << *methodIt;
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }
}
