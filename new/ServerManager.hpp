#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include <cstdlib>
#include <iostream>
#include <unistd.h>


class ServerManager 
{
  private:
    std::vector<ServerConfig> m_configs;
    std::vecctor<int> m_listenSocket;
    
    std::map<int, ServerConfig> m_configMap;
    std::map<int, struct sockaddr_in> m_addressMap;
  public:
    ServerManager();
    ~ServerManager();
    
    void SetupServer(const std::vector<ServerConfig>& config);
    void run();
}

void ServerManager::SetupServer(const std::vector<ServerConfig>& config)
{
  this->m_config = config;
  std::vector<ServerConfig>::const_iterator start = config.start();
  std::vector<ServerConfig>::const_iterator end = config.end();

  for(std::vector<ServerConfig>::const_iterator it = start; it != end; ++it)
  {
    if (it->port)
    {
      int new_socket = socket(AF_INET, SOCK_STREAM, 0);
      if (new_socket < 0)
      {
        std::cerr << "There is an error in the socket creation" << std::endl;
        continue; // skipping this server and going to the next one
      }
      // setsockopt()
      int y = 1;
      if (setsockopt(new_socket, SOL_SOCKET,SO_REUSEADDR, &y, sizeof(int)) < 0)
      {
        std::cerr << "Error: failed setsock NEXT" << std::endl;
        close(new_socket);
        continue;
      }
      if (fcntl(new_socket, F_SETFL, O_NONBLOCK) < 0)
      {
        std::cerr << "Error fcntl failed like my grades" << std::endl;
        close(new_socket);
        continue;
      }
      struct sockaddr_in server_addr;
      memset(&server_addr, 0, sizeof(server_addr));
      server_addr.sin_family = AF_INET;
      server_addr.sin_port = htons(it->port);
      server_addr.sin_addr.s_addr = htonl(INADD_ANY);

      if (bind(new_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
      {
        std::cerr << "Error in the binding for the server : " << it->serverName;
        close(new_socket);
        continue;
      }

      if (listetn(new_socket, 128) < 0)
      {
        std::cerr << "error : listen failed for the server: " << it->serverName;
        close(new_socket);
        continue;
      }

      m_listenSocket.push_back(new_socket);
      m_configMap[new_socket] = *it;
      m_addressMap[new_socket] = server_addr;
      std::cout << "--------YAHOO THE SERVER " << it->serverName << " Is set " << std::endl;
    }
    else 
    {
      std::cerr << "The port for the server " << it->serverName << "isn't set" << std::endl;
      continue;
    }
  }

  std::cout << "----- The setuping is done -----" << std::endl;
}

#endif
