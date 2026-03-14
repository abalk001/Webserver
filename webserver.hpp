#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

# define PORT 8080
# define QUEU 5

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

class Server 
{
private:
  const int m_server_fd;
  int m_client_fd;
  struct sockaddr_in m_server_add;
  struct sockaddr_in m_client_add;
public:
  Server();
  ~Server();
  int GetSocketfd();
  struct sockaddr_in GetSockaddrin();
  int setuping();
  int setuping_recv();
  class SocketException : std::exception 
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



// int setuping(int *socket, struct sockaddr_in *server_add);
// int setuping_recv(int *socket, struct sockaddr *client_addr);
void printing_vect(const std::vector<char> &buff, ssize_t bytes_read);

std::string search_find(const std::string &word, const std::vector<char> &stc, ssize_t *bytes_read);
std::string sending(std::string &index);
std::string get_content(const std::string& filename);
#endif 
