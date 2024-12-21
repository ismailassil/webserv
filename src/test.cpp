// #include "Config.hpp"

// void Config::validateToken(const std::string& token, const std::string& context)
// {
//     static const std::set<std::string> validGlobalTokens = {"server"};
//     static const std::set<std::string> validServerTokens = {"listen", "server_name", "location"};
//     static const std::set<std::string> validLocationTokens = {"root", "autoindex", "index", "upload_path"};

//     const std::set<std::string>* validTokens = NULL;

//     if (context == "global") {
//         validTokens = &validGlobalTokens;
//     } else if (context == "server") {
//         validTokens = &validServerTokens;
//     } else if (context == "location") {
//         validTokens = &validLocationTokens;
//     } else {
//         throw std::runtime_error("Error: Unknown context '" + context + "'");
//     }
//     if (validTokens->find(token) == validTokens->end()) {
//         throw std::runtime_error("Error: Invalid token '" + token + "' in context '" + context + "'");
//     }
// }

// int Config::extractLocationContext(const std::vector<std::string>& tokens, int startIndex, Location& location)
// {
//     std::map<std::string, std::vector<std::string> > directives;

//     for (int i = startIndex; i < tokens.size(); ++i) {
//         if (tokens[i] == "}")
//         {
//             location.setDirectives(directives);
//             return i;
//         }
//         else
//         {
//             std::string key = tokens[i];
//             if (i + 1 < tokens.size() && tokens[i + 1] != ";" && tokens[i + 1] != "{" && tokens[i + 1] != "}") {
//                 std::string value = tokens[i + 1];
//                 directives[key].push_back(value);
//                 i++;
//             }
//             else
//                 throw std::runtime_error("Error: Invalid directive syntax near '" + key + "'");
//         }
//     }

//     throw std::runtime_error("Error: Missing '}' to close 'location' context.");
// }

// int Config::extractServerContext(const std::vector<std::string>& tokens, int startIndex, Server& server)
// {
//     std::map<std::string, std::vector<std::string> > directives;

//     for (int i = startIndex; i < tokens.size(); ++i)
//     {
//         if (tokens[i] == "}")
//         {
//             server.setDirectives(directives);
//             return i;
//         }
//         else if (tokens[i] == "location")
//         {
//             if (i + 1 < tokens.size() && tokens[i + 1] == "{")
//             {
//                 Location location;
//                 i = extractLocationContext(tokens, i + 2, location);
//                 server.addLocation(location);
//             }
//             else
//                 throw std::runtime_error("Error: 'location' must be followed by '{'");
//         }
//         else
//         {
//             std::string key = tokens[i];
//             if (i + 1 < tokens.size() && tokens[i + 1] != ";" && tokens[i + 1] != "{" && tokens[i + 1] != "}") {
//                 std::string value = tokens[i + 1];
//                 directives[key].push_back(value);
//                 i++;
//             } else {
//                 throw std::runtime_error("Error: Invalid directive syntax near '" + key + "'");
//             }
//         }
//     }

//     throw std::runtime_error("Error: Missing '}' to close 'server' context.");
// }

// void Config::parseTokens(const std::vector<std::string>& tokens)
// {
//     Server currentServer;
//     bool serverFound = false;

//     for (int i = 0; i < tokens.size(); i++)
//     {
//         if (tokens[i] == "server")
//         {
//             if (i + 1 < tokens.size() && tokens[i + 1] == "{")
//             {
//                 serverFound = true;
//                 i = extractServerContext(tokens, i + 2, currentServer);
//             }
//             else
//                 throw std::runtime_error("Error: 'server' must be followed by '{'");
//         }
//         else
//             throw std::runtime_error("Error: Unknown context");
//     }

//     if (!serverFound)
//         throw std::runtime_error("Error: No 'server' context found in the configuration.");
//     servers.push_back(currentServer);
// }
