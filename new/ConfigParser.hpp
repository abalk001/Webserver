#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <vector>
#include <map>
#include <iostream>
#include <string>
#include <sstream>

struct LocationConfig {
  std::string path; // the route identifier 
  std::vector<std::string> allowedMethodes; // for Get and so on 
  std::string redirectUrl; // HTTP redirection 
  std::string root; // Where requested file should be locared 
  std::string indexFile; // Default file (index.html)
  std::string upladPath; // store the loc for the upload 
  std::string cgiExtension;
  std::string cgiPath;
  bool autoindex;
  bool uploadEnable;

};



struct ServerConfig 
{
  std::string host;
  int port; // THis is port where the server will listen. I think it's pretty clear 
  std::string serverName; // no need I guess
  size_t clientMax; // same here 
  std::map<int, std::string> errorPage; // 
  std::vector<LocationConfig> locations;
  std::string root; 
  std::string indexFile;

};

template<typename T>
bool extractValue(const std::vector<std::string>& tokens, size_t& i,
                  T& outValue)
{
  if (i >= tokens.size()) return false;
  i++;
  std::stringstream ss(tokens[i]);
  ss >> outValue;

  i++;
  if (i >= tokens.size() || tokens[i] != ";")
  {
    std::cerr << "Error here 3" << std::endl;
    return false;
  }
  return true;
}



class ConfigParser {
  private:
    std::vector<ServerConfig> m_servers;
    typedef void (ConfigParser::*ServerHandler)(ServerConfig&, size_t&, const std::vector<std::string>&);
    typedef void (ConfigParser::*LocationHandler)(LocationConfig&, size_t&, const std::vector<std::string>&);
    std::map<std::string, ServerHandler> m_serverDispatch;
    std::map<std::string, LocationHandler> m_locationDispatch;
    void handleListen(ServerConfig& server, size_t& i, const std::vector<std::string>&);
    void handleServer(ServerConfig& server, size_t& i, const std::vector<std::string>&);
    void handleHost(ServerConfig& server, size_t& i, const std::vector<std::string>&);
    void handleRoot(ServerConfig& server, size_t& i, const std::vector<std::string>&);
    void handleClient(ServerConfig& server, size_t& i, const std::vector<std::string>&);
    void handleIndex(ServerConfig& server, size_t& i, const std::vector<std::string>&);
    void handleErrorPage(ServerConfig& server, size_t& i, const std::vector<std::string>&);
    void handleLocation(ServerConfig& server, size_t& i, const std::vector<std::string>&);
    void handleLocationAllow(LocationConfig& location, size_t& i, const std::vector<std::string>&);
    void handleLocationAutoIndex(LocationConfig& location, size_t& i, const std::vector<std::string>&);
    void handleLocationIndex(LocationConfig& location, size_t& i, const std::vector<std::string>&);
    void handleLocationRoot(LocationConfig& location, size_t& i, const std::vector<std::string>&);
    void handleLocationReturn(LocationConfig& location, size_t& i, const std::vector<std::string>&);
    void handleLocationUploadPath(LocationConfig& location, size_t& i, const std::vector<std::string>&);
    void handleLocationUploadEnable(LocationConfig& location, size_t& i, const std::vector<std::string>&);
    void handleLocationCgiPath(LocationConfig& location, size_t& i, const std::vector<std::string>&);
    void handleLocationCgiExt(LocationConfig& location, size_t& i, const std::vector<std::string>&);
    std::vector<std::string> m_helpingParser(const std::string& filename);
    void m_parsingServerBlock(const std::vector<std::string>& tokens, size_t &i);
    void m_parsingLocationBlock(ServerConfig& server, size_t& i, const std::vector<std::string>& tokens);
    bool extractList(const std::vector<std::string>& tokens, size_t& i, std::vector<std::string>& out);
    bool extractBool(const std::vector<std::string>& tokens, size_t& i, bool& out);
    bool extractJoined(const std::vector<std::string>& tokens, size_t& i, std::string& out);
  public:
    ConfigParser();
    ~ConfigParser();
    bool parsefile(const std::string& filename);
    std::vector<ServerConfig> getServer() const;
};


#endif
