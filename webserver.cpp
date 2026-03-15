#include "webserver.hpp"
#include <errno.h>

Server::Server(int port, const std::string& root) : m_listen_fd(-1), m_root(root)
{
  m_listen_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (m_listen_fd < 0)
    throw SocketException();

  memset(&m_server_add, 0, sizeof(m_server_add));
  m_server_add.sin_family = AF_INET;
  m_server_add.sin_port = htons(port);
  m_server_add.sin_addr.s_addr = INADDR_ANY;

  setNonBlocking(m_listen_fd);

  int y = 1;
  if (setsockopt(m_listen_fd, SOL_SOCKET, SO_REUSEADDR, &y, sizeof(int)) == -1)
    throw SocketException();
  if (bind(m_listen_fd, (struct sockaddr *)&m_server_add, sizeof(struct sockaddr)) < 0)
    throw SocketException();
  if (listen(m_listen_fd, BACKLOG) < 0)
    throw SocketException();

  struct pollfd pfd;
  pfd.fd = m_listen_fd;
  pfd.events = POLLIN;
  pfd.revents = 0;
  m_fds.push_back(pfd);

  std::cout << "Server listening on port " << port << std::endl;
}

Server::~Server()
{
  for (size_t i = 0; i < m_fds.size(); ++i)
    close(m_fds[i].fd);
}

void Server::setNonBlocking(int fd)
{
  int flags = fcntl(fd, F_GETFL, 0);
  if (flags < 0)
    throw SocketException();
  if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0)
    throw SocketException();
}

void Server::run()
{
  while (true)
  {
    int ready = poll(&m_fds[0], m_fds.size(), -1);
    if (ready < 0)
    {
      if (errno == EINTR)
        continue;
      throw SocketException();
    }

    size_t i = 0;
    while (i < m_fds.size())
    {
      struct pollfd current = m_fds[i];

      if (current.revents & (POLLHUP | POLLERR | POLLNVAL))
      {
        closeClient(current.fd);
        continue;
      }

      if (current.revents & POLLIN)
      {
        if (current.fd == m_listen_fd)
          acceptClient();
        else
          handleRead(current.fd);
      }

      if (i < m_fds.size() && (current.revents & POLLOUT))
        handleWrite(current.fd);

      ++i;
    }
  }
}

void Server::acceptClient()
{
  while (true)
  {
    struct sockaddr_in client_addr;
    socklen_t sin_size = sizeof(client_addr);
    int client_fd = accept(m_listen_fd, (struct sockaddr *)&client_addr, &sin_size);
    if (client_fd < 0)
    {
      if (errno == EAGAIN || errno == EWOULDBLOCK)
        break;
      throw SocketException();
    }

    setNonBlocking(client_fd);
    struct pollfd pfd;
    pfd.fd = client_fd;
    pfd.events = POLLIN;
    pfd.revents = 0;
    m_fds.push_back(pfd);
    m_clients[client_fd] = ClientState();
  }
}

bool Server::isRequestComplete(const std::string& rawRequest)
{
  return rawRequest.find("\r\n\r\n") != std::string::npos;
}

bool Server::parseRequestLine(const std::string& rawRequest, std::string& method, std::string& path)
{
  std::string::size_type end = rawRequest.find("\r\n");
  if (end == std::string::npos)
    return false;
  std::string firstLine = rawRequest.substr(0, end);
  std::istringstream iss(firstLine);
  std::string version;
  if (!(iss >> method >> path >> version))
    return false;
  return true;
}

std::string Server::buildResponse(const std::string& rawRequest)
{
  std::string method;
  std::string path;
  if (!parseRequestLine(rawRequest, method, path))
    return "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";

  if (method != "GET" && method != "HEAD")
    return "HTTP/1.1 405 Method Not Allowed\r\nAllow: GET, HEAD\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";

  if (path == "/")
    path = "/index.html";

  std::string filePath = m_root;
  if (!filePath.empty() && filePath[filePath.size() - 1] != '/')
    filePath += "/";
  if (path.size() > 0 && path[0] == '/')
    filePath += path.substr(1);
  else
    filePath += path;

  std::string response = sendingI(filePath);

  if (method == "HEAD")
  {
    std::string::size_type pos = response.find("\r\n\r\n");
    if (pos != std::string::npos)
      response = response.substr(0, pos + 4);
  }

  return response;
}

void Server::handleRead(int fd)
{
  char buffer[4096];
  while (true)
  {
    ssize_t bytes = recv(fd, buffer, sizeof(buffer), 0);
    if (bytes < 0)
    {
      if (errno == EAGAIN || errno == EWOULDBLOCK)
        break;
      closeClient(fd);
      return;
    }
    if (bytes == 0)
    {
      closeClient(fd);
      return;
    }

    m_clients[fd].request.append(buffer, buffer + bytes);

    if (isRequestComplete(m_clients[fd].request))
    {
      m_clients[fd].response = buildResponse(m_clients[fd].request);
      m_clients[fd].bytes_sent = 0;

      for (size_t i = 0; i < m_fds.size(); ++i)
      {
        if (m_fds[i].fd == fd)
        {
          m_fds[i].events = POLLOUT;
          break;
        }
      }
      break;
    }
  }
}

void Server::handleWrite(int fd)
{
  ClientState &state = m_clients[fd];
  const std::string &resp = state.response;

  ssize_t sent = send(fd, resp.c_str() + state.bytes_sent, resp.size() - state.bytes_sent, 0);
  if (sent < 0)
  {
    if (errno == EAGAIN || errno == EWOULDBLOCK)
      return;
    closeClient(fd);
    return;
  }

  state.bytes_sent += static_cast<size_t>(sent);
  if (state.bytes_sent >= resp.size())
    closeClient(fd);
}

void Server::closeClient(int fd)
{
  close(fd);
  m_clients.erase(fd);

  for (size_t i = 0; i < m_fds.size(); ++i)
  {
    if (m_fds[i].fd == fd)
    {
      m_fds.erase(m_fds.begin() + i);
      break;
    }
  }
}


