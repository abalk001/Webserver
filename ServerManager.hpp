#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP 

#include <vector>
#include <map>
#include <string>

class ServerManager 
{
private:
  std::string m_server_name;
  std::vector<int> m_listsock;
public:
  ServerManager();
  ~ServerManager();
  setuping()
  {
    for
  }
};


class HttpRequest
{
private:
  std::string m_methods;
  std::string m_URL;
  std::string m_version;
  std::map<std::string, std::string> m_headers;
  std::string m_body;
public:
  bool parse(const std::string& rawData);
};



#endif 
