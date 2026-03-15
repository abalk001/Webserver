#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <vector>
#include <map>
#include <iostream>


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
  bool upladEnable;

  LocationConfig(): autoindex(false), upladPath(false) {}
};



struct ServerConfig 
{
  std::string host;
  int port;
  std::string serverName;
  size_t clientMax;
  std::map<int, std::string> errorPage;
  std::vector<LocationConfig> locations;

  ServerConfig() : port(80), clientMax(3) {}
};


class ConfigParser {
  private:
    std::vector<ServerConfig> m_servers;
  public:
    ConfigParser();
    ~ConfigParser();
    bool parsefile(const std::string& filename);
    std::vector<ServerConfig> getServer() const;
};



#endif
