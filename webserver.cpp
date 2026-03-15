#include "webserver.hpp"


Server::Server() : m_server_fd(socket(AF_INET, SOCK_STREAM, 0)), m_buff(1024)
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

void Server::sending(ssize_t bytes_read)
{
  std::string ext = ".html";
  std::string filename =  Server::search_find(ext, bytes_read);
  if (filename.size() == 0)
  {
    ext = ".css";
    filename = Server::search_find(ext, bytes_read);
  }
  std::cout << "The filename asked for: ";
  std::cout << filename << std::endl;

  ////////////
  std::string http_response = sendingI(filename);
  ssize_t dataSent = send(m_client_fd, http_response.c_str(), http_response.size(),0);
  if (dataSent < 0)
  {
    std::cerr << "Error in the sent" << std::endl;
    close(m_server_fd);
    close(m_client_fd);
    throw SendingException();
  }
  if (static_cast<size_t>(dataSent) == http_response.size())
    std::cout << "We cool"<< std::endl;
  else 
    std::cout << "Only " << dataSent << "was sent over " << http_response.size() << std::endl;
  close(m_client_fd);
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

ssize_t Server::receiving()
{
  ssize_t bytes_read = recv(m_client_fd, m_buff.data(), m_buff.size(), 0);
  std::cout << "Recevied request" << std::endl;
  printing_vect(m_buff, bytes_read);
  return bytes_read;
}

std::string Server::search_find(std::string &word, ssize_t bytes_read)
{
  std::vector<char>::iterator end_it = m_buff.begin() + bytes_read;
  std::vector<char>::iterator it = std::search(m_buff.begin(), end_it, 
                                                      word.begin(), word.end());
  if (it != end_it)
  {
    std::vector<char>::const_iterator start = it;
    std::vector<char>::const_iterator end = it;

    while (start != m_buff.begin() && *start != ' ')
      start--;
    if(*start == ' ')
      start++;
    while (end != end_it && *end != ' ' 
          && *end != '\r' && *end != '\n')
    {end++;}

    return std::string(start, end);
  }

  return "";

}


