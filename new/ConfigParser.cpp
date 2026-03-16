#include "ConfigParser.hpp"
#include <fstream>
#include <sstream>


ConfigParser::ConfigParser()
{
  m_serverDispatch["listen"] = &ConfigParser::handleListen;
  m_serverDispatch["server"] = &ConfigParser::handleServer;
}
ConfigParser::~ConfigParser()
{}
void ConfigParser::handleServer(ServerConfig& server, size_t& i, const std::vector<std::string>& tokens)
{
    // TODO: implement
    (void)server;
    (void)i;
    (void)tokens;
}
std::vector<std::string> ConfigParser::m_helpingParser(const std::string& filename)
{
  std::ifstream file(filename.c_str());
  std::vector<std::string> tokens;


  std::string line;
  while(std::getline(file, line))
  {
    size_t commentPos = line.find('#');
    if (commentPos != std::string::npos) // removing the comments 
      line = line.substr(0, commentPos);
    std::string alpha;
    for (size_t i = 0; i < line.size();i++)
    {
      if (line[i] == '{' || line[i] == '}' || line[i] == ';')
      {
        alpha += ' ';
        alpha += line[i];
        alpha += ' ';
      }
      else 
        alpha += line[i];
    }

    std::istringstream iss(alpha);
    std::string word;
    while(iss >> word) 
      tokens.push_back(word);
    //std::cout << alpha << std::endl;
  }
  return tokens;
} 


void ConfigParser::m_parsingServerBlock(const std::vector<std::string>& tokens, size_t& i)
{
  ServerConfig newServ;
  while (i < tokens.size() && tokens[i] != "}")
  {
    std::map<std::string, ServerHandler>::iterator it = m_serverDispatch.find(tokens[i]);
    if (it != m_serverDispatch.end())
    {
      (this->*(it->second))(newServ, i, tokens);
    }
    else 
    {
      std::cerr << "Error" << tokens[i] << std::endl;
    }
    i++;
  }
  m_servers.push_back(newServ);
}

void ConfigParser::handleListen(ServerConfig& server, size_t& i, const std::vector<std::string>& tokens)
{
  i++;
  std::cout << "port: " << std::atoi(tokens[i].c_str());
}
bool ConfigParser::parsefile(const std::string& fileame)
{
  // check if it can open the file or not 

  std::vector<std::string> tokens;
  tokens = m_helpingParser(fileame);
  if(tokens.empty())
    return false;
  for (size_t i = 0; i < tokens.size(); i++)
  {
    if (tokens[i] == "server")
    {
      if (i + 1 < tokens.size() && tokens[i + 1] == "{")
      {
        i += 2; // skiping the server and the { 
        m_parsingServerBlock(tokens,i);
      }
      else 
      {
        std::cerr << "Error here" << std::endl;
        return false;
      }
    }
    else 
    {
      std::cerr << "Error here 2" << std::endl;
      return true;
    }
  }
  std::cout << m_servers[0].port << std::endl;
    return true;
}
