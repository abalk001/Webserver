#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

# define PORT 8080
# define BACKLOG 64

#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <cstring>
#include <netinet/in.h>
#include <string>
#include <vector>
#include <algorithm> 
#include <fstream>
#include <iterator>
#include <exception>  
#include <sstream>
#include <fcntl.h>
#include <poll.h>
#include <map>




struct ClientState
{
  std::string request;
  std::string response;
  size_t      bytes_sent;
};

class Server 
{
private:
  int m_listen_fd;
  struct sockaddr_in m_server_add;
  std::vector<struct pollfd> m_fds;
  std::map<int, ClientState> m_clients;
  std::string m_root;

  void setNonBlocking(int fd);
  void acceptClient();
  void handleRead(int fd);
  void handleWrite(int fd);
  void closeClient(int fd);
  std::string buildResponse(const std::string& rawRequest);
  bool isRequestComplete(const std::string& rawRequest);
  bool parseRequestLine(const std::string& rawRequest, std::string& method, std::string& path);

public:
  Server(int port = PORT, const std::string& root = ".");
  ~Server();
  void run();

  class SocketException : std::exception \
  {
  public:
    const char *what() const throw() {
      return "Socket failed \n";
    }
  };
};

template <typename T>
std::string to_string(T value)
{
  std::ostringstream oss;
  oss << value;
  return oss.str();
}

std::string sendingI(const std::string &path);
std::string get_content(const std::string& filename);
bool fileExists(const std::string& filename);
#endif 
