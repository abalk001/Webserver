#include "ConfigParser.hpp"
#include <fstream>
#include <sstream>

void ConfigParser::handleListen(ServerConfig& server, size_t& i,
                                const std::vector<std::string>& tokens)
{
  extractValue(tokens, i , server.port);
  std::cout << "port : "<< server.port << std::endl;
}
void ConfigParser::handleHost(ServerConfig& server, size_t& i,
                              const std::vector<std::string>& tokens)
{
  extractValue(tokens, i , server.host);
  std::cout << "host: " << server.host << std::endl;
}
void ConfigParser::handleServer(ServerConfig& server, size_t& i,
                                const std::vector<std::string>& tokens)
{
  extractValue(tokens, i, server.serverName);
  std::cout << "serverName: " << server.serverName << std::endl;
}



