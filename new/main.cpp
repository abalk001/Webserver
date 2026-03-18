#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include "ConfigParser.hpp"
#include <sys/socket.h>
// this fct is working perfectly !!
#include <netinet/in.h>
#include "ServerManager.hpp"





int main(void)
{
  ConfigParser conf;
  conf.parsefile("default.conf");
  const std::vector<ServerConfig> ser = conf.getServer();
  ServerManager server;
  server.SetupServer(ser);
  return 0;
}
