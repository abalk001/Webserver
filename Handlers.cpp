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


void ConfigParser::handleClient(ServerConfig& server, size_t&i, 
                              const std::vector<std::string>& tokens)
{
  extractValue(tokens, i , server.clientMax);
  std::cout << "Client Max : " << server.clientMax << std::endl;
}


void ConfigParser::handleRoot(ServerConfig& server, size_t& i,
                    const std::vector<std::string>& tokens)

{
  extractValue(tokens, i, server.root);
  std::cout << "Server root: " << server.root << std::endl;
}

void ConfigParser::handleIndex(ServerConfig& server, size_t& i, 
                  const std::vector<std::string>& tokens)
{
  extractValue(tokens, i, server.indexFile);
  std::cout << "index file: " << server.indexFile << std::endl;
}

void ConfigParser::handleErrorPage(ServerConfig& server, size_t& i,
                                   const std::vector<std::string>& tokens)
{
  size_t start = i + 1;
  size_t cursor = start;
  while (cursor < tokens.size() && tokens[cursor] != ";")
    cursor++;
  if (cursor <= start + 1 || cursor >= tokens.size())
  {
    std::cerr << "Error parsing error_page directive" << std::endl;
    return;
  }
  std::string pagePath = tokens[cursor - 1];
  for (size_t idx = start; idx < cursor - 1; ++idx)
  {
    int code = 0;
    std::stringstream ss(tokens[idx]);
    ss >> code;
    if (ss.fail())
    {
      std::cerr << "Invalid error code: " << tokens[idx] << std::endl;
      continue;
    }
    server.errorPage[code] = pagePath;
  }
  i = cursor;
}

void ConfigParser::handleLocation(ServerConfig& server, size_t& i,
                                  const std::vector<std::string>& tokens)
{
  m_parsingLocationBlock(server, i, tokens);
}

void ConfigParser::handleLocationAllow(LocationConfig& location, size_t& i,
                                       const std::vector<std::string>& tokens)
{
  location.allowedMethodes.clear();
  extractList(tokens, i, location.allowedMethodes);
  std::cout << "Allowed methods: ";
  std::vector<std::string>::iterator start = location.allowedMethodes.begin();
  for (std::vector<std::string>::iterator it = start; it != location.allowedMethodes.end(); ++it)
  {
    std::cout << *it << " ";
  }
  std::cout << std::endl;
}

void ConfigParser::handleLocationAutoIndex(LocationConfig& location, size_t& i,
                                           const std::vector<std::string>& tokens)
{
  extractBool(tokens, i, location.autoindex);
  std::cout << "Autoindex: " << (location.autoindex ? "on" : "off") << std::endl;
}

void ConfigParser::handleLocationIndex(LocationConfig& location, size_t& i,
                                       const std::vector<std::string>& tokens)
{
  extractValue(tokens, i, location.indexFile);
  std::cout << "Location index file: " << location.indexFile << std::endl;
}

void ConfigParser::handleLocationRoot(LocationConfig& location, size_t& i,
                                      const std::vector<std::string>& tokens)
{
  extractValue(tokens, i, location.root);

  std::cout << "Location root: " << location.root << std::endl;
}

void ConfigParser::handleLocationReturn(LocationConfig& location, size_t& i,
                                        const std::vector<std::string>& tokens)
{
  extractValue(tokens, i, location.redirectUrl);
  std::cout << "Location redirect URL: " << location.redirectUrl << std::endl;
}

void ConfigParser::handleLocationUploadPath(LocationConfig& location, size_t& i,
                                            const std::vector<std::string>& tokens)
{
  extractValue(tokens, i, location.upladPath);
  std::cout << "Location upload path: " << location.upladPath << std::endl;
}

void ConfigParser::handleLocationUploadEnable(LocationConfig& location, size_t& i,
                                              const std::vector<std::string>& tokens)
{
  extractBool(tokens, i, location.uploadEnable);
}

void ConfigParser::handleLocationCgiPath(LocationConfig& location, size_t& i,
                                         const std::vector<std::string>& tokens)
{
  extractJoined(tokens, i, location.cgiPath);
  std::cout << "Location CGI path: " << location.cgiPath << std::endl;
}

void ConfigParser::handleLocationCgiExt(LocationConfig& location, size_t& i,
                                        const std::vector<std::string>& tokens)
{
  extractJoined(tokens, i, location.cgiExtension);
  std::cout << "Location CGI extension: " << location.cgiExtension << std::endl;
}



