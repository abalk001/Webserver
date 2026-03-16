#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <vector>
#include <map>
#include <iostream>
#include <string>

struct LocationConfig {
  std::string path;
  std::vector<std::string> allowedMethodes; // for Get and so on 
  std::string redirectUrl; // HTTP redirection 
  std::string root; // Where the file should be locared 
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
  int port;
  std::string serverName;
  size_t clientMax;
  std::map<int, std::string> errorPage;
  std::vector<LocationConfig> locations;

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
    std::map<std::string, ServerHandler> m_serverDispatch;
    void handleListen(ServerConfig& server, size_t& i, const std::vector<std::string>&);
    void handleServer(ServerConfig& server, size_t& i, const std::vector<std::string>&);
    void handleHost(ServerConfig& server, size_t& i, const std::vector<std::string>&);
    std::vector<std::string> m_helpingParser(const std::string& filename);
    void m_parsingServerBlock(const std::vector<std::string>& tokens, size_t &i);
  public:
    ConfigParser();
    ~ConfigParser();
    bool parsefile(const std::string& filename);
    std::vector<ServerConfig> getServer() const;
};


#endif
