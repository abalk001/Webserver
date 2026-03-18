#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <sys/socket.h>
#include <map>
#include <vector>
#include <netinet/in.h>
#include "ConfigParser.hpp"
class ServerManager 
{
  private:
    std::vector<ServerConfig> m_configs;
    std::vector<int> m_listenSocket;
    
    std::map<int, ServerConfig> m_configMap;
    std::map<int, struct sockaddr_in> m_addressMap;
  public:
    ServerManager();
    ~ServerManager();
    
    void SetupServer(const std::vector<ServerConfig>& config);
    
};

#endif
