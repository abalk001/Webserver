#include "webserver.hpp"


Server::Server() : m_server_fd(socket(AF_INET, SOCK_STREAM, 0))
{
  if (this->m_server_fd < 0)
    throw SocketException();
  memset(&this->m_server_add, 0, sizeof(this->m_server_add));
  this->m_server_add.sin_family = AF_INET;
  this->m_server_add.sin_port = htons(PORT);
  this->m_server_add.sin_addr.s_addr = INADDR_ANY;
}

Server::~Server()
{
  close(m_server_fd);
}

int Server::GetSocketfd()
{
  return this->m_server_fd;
}

struct sockaddr_in Server::GetSockaddrin()
{
  return this->m_server_add;
}

int Server::setuping_recv()
{
  socklen_t sin_size = sizeof(m_client_add);
  m_client_fd = accept(m_server_fd, (struct sockaddr *)&m_client_add, &sin_size);
  if (m_client_fd < 0)
  {
    std::cerr << "There is an issue with accept" << std::endl;
    return -1;
  }

  return m_client_fd;
  
}

int Server::setuping()
{

  int y = 1;
  
  if (setsockopt(m_server_fd, SOL_SOCKET, SO_REUSEADDR, &y, sizeof(int)) == -1)
  {
    std::cerr << "error in the setsockopt"<< std::endl;
    return -1;
  }
  if (bind(m_server_fd, (struct sockaddr *)&m_server_add, sizeof(struct sockaddr)))
  {
    std::cerr << "Error in the binding" << std::endl;
    return -1;
  }
  
  if (listen(m_server_fd, QUEU) < 0)
  {
    std::cerr << "error in the listening part" << std::endl;
    return -1;
  }
  std::cout << "The sever is listening" << std::endl;

  return 1;
}


