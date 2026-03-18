#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include "ConfigParser.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
// this fct is working perfectly !!
#include "ServerManager.hpp"





int main(void)
{
  ConfigParser conf;
  conf.parsefile("default.conf");
  std::vector<ServerConfig> ser = conf.getServer();
  ServerManager server;
  server.SetupServers(ser);
  return 0;
}
