#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include "ConfigParser.hpp"

// this fct is working perfectly !!




class ServerManager
{
private:
  std::vector<ServerConfig> m_configs;
  std::vector<int> m_listenSockets;
  
public:
  ServerManager();
  ~ServerManager();
  void SetupServers(const std::vector<ServerConfig>& configs);
  void run();
};



int main(void)
{
  ConfigParser conf;
  conf.parsefile("default.conf");
  std::vector<ServerConfig> server = conf.getServer();
  
  return 0;
}
