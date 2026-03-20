#include "ConfigParser.hpp"
#include <fstream>
#include <sstream>


ConfigParser::ConfigParser()
{
  m_serverDispatch["listen"] = &ConfigParser::handleListen;
  m_serverDispatch["server_name"] = &ConfigParser::handleServer;
  m_serverDispatch["host"] = &ConfigParser::handleHost;
  m_serverDispatch["root"] = &ConfigParser::handleRoot;
  m_serverDispatch["client_max_body_size"] = &ConfigParser::handleClient;
  m_serverDispatch["index"] = &ConfigParser::handleIndex;
  m_serverDispatch["error_page"] = &ConfigParser::handleErrorPage;
  m_serverDispatch["location"] = &ConfigParser::handleLocation;

  m_locationDispatch["allow_methods"] = &ConfigParser::handleLocationAllow;
  m_locationDispatch["autoindex"] = &ConfigParser::handleLocationAutoIndex;
  m_locationDispatch["index"] = &ConfigParser::handleLocationIndex;
  m_locationDispatch["root"] = &ConfigParser::handleLocationRoot;
  m_locationDispatch["return"] = &ConfigParser::handleLocationReturn;
  m_locationDispatch["upload_store"] = &ConfigParser::handleLocationUploadPath;
  m_locationDispatch["upload_enable"] = &ConfigParser::handleLocationUploadEnable;
  m_locationDispatch["cgi_path"] = &ConfigParser::handleLocationCgiPath;
  m_locationDispatch["cgi_ext"] = &ConfigParser::handleLocationCgiExt;
}
ConfigParser::~ConfigParser()
{}

std::vector<std::string> ConfigParser::m_helpingParser(const std::string& filename)
{
  // We tokenize the shit out this 
  std::ifstream file(filename.c_str());
  std::vector<std::string> tokens;

  if (!file.is_open())
  {
    std::cerr << "Could not open config file: " << filename << std::endl;
    return tokens;
  }


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
  newServ.port = 0;
  newServ.clientMax = 0;
  newServ.host.clear();
  newServ.serverName.clear();
  newServ.root.clear();
  newServ.indexFile.clear();
  while (i < tokens.size() && tokens[i] != "}")
  {
    std::map<std::string, ServerHandler>::iterator it = m_serverDispatch.find(tokens[i]);
    if (it != m_serverDispatch.end())
    {
      (this->*(it->second))(newServ, i, tokens); // run the fct we just found in the map (synthax from hell)
    }
    else 
    {
      std::cerr << "Error in finding the fct in cpp for : " << tokens[i] << std::endl;
    }
    i++;
  }
  m_servers.push_back(newServ);
}


bool ConfigParser::parsefile(const std::string& fileame)
{
  // check if it can open the file or not 

  std::vector<std::string> tokens;
  tokens = m_helpingParser(fileame); // tokenzation
  if(tokens.empty())
    return false;
  for (size_t i = 0; i < tokens.size(); i++)
  {
    if (tokens[i] == "server")
    {
      if (i + 1 < tokens.size() && tokens[i + 1] == "{")
      {
        i += 2; // skiping the server and the { 
        m_parsingServerBlock(tokens,i);// maping the config into our struct
      }
      else 
      {
        std::cerr << "Error here in config " << std::endl;
        return false;
      }
    }
    else 
    {
      std::cerr << "Error here 2: no server found" << std::endl;
      return false;
    }
  }
  if (m_servers.empty())
    return false;
  return true;
}

std::vector<ServerConfig> ConfigParser::getServer() const
{
  return m_servers;
}


bool ConfigParser::extractList(const std::vector<std::string>& tokens, size_t& i,
                               std::vector<std::string>& out)
{
  i++;
  while (i < tokens.size() && tokens[i] != ";")
  {
    out.push_back(tokens[i]);
    i++;
  }
  if (i >= tokens.size() || tokens[i] != ";")
  {
    std::cerr << "Error parsing list value" << std::endl;
    return false;
  }
  return true;
}

bool ConfigParser::extractBool(const std::vector<std::string>& tokens, size_t& i,
                               bool& out)
{
  i++;
  if (i >= tokens.size())
    return false;
  const std::string& word = tokens[i];
  if (word == "on" || word == "1" || word == "true")
    out = true;
  else if (word == "off" || word == "0" || word == "false")
    out = false;
  else
  {
    std::cerr << "Unexpected bool token: " << word << std::endl;
    return false;
  }
  i++;
  if (i >= tokens.size() || tokens[i] != ";")
  {
    std::cerr << "Missing semicolon after bool directive" << std::endl;
    return false;
  }
  return true;
}

bool ConfigParser::extractJoined(const std::vector<std::string>& tokens, size_t& i,
                                 std::string& out)
{
  i++;
  std::ostringstream oss;
  bool first = true;
  while (i < tokens.size() && tokens[i] != ";")
  {
    if (!first)
      oss << ' ';
    oss << tokens[i];
    first = false;
    i++;
  }
  if (i >= tokens.size() || tokens[i] != ";")
  {
    std::cerr << "Missing semicolon for joined directive" << std::endl;
    return false;
  }
  out = oss.str();
  return true;
}

void ConfigParser::m_parsingLocationBlock(ServerConfig& server, size_t& i,
                                          const std::vector<std::string>& tokens)
{
  if (i + 1 >= tokens.size())
  {
    std::cerr << "location missing path" << std::endl;
    return;
  }
  LocationConfig location;
  location.path = tokens[i + 1];
  location.autoindex = false;
  location.uploadEnable = false;
  location.redirectUrl.clear();
  location.allowedMethodes.clear();
  location.indexFile.clear();
  location.root.clear();
  location.upladPath.clear();
  location.cgiExtension.clear();
  location.cgiPath.clear();

  if (i + 2 >= tokens.size() || tokens[i + 2] != "{")
  {
    std::cerr << "location block missing opening brace" << std::endl;
    return;
  }
  i += 3; // move to first token inside location
  while (i < tokens.size() && tokens[i] != "}")
  {
    std::map<std::string, LocationHandler>::iterator it = m_locationDispatch.find(tokens[i]);
    if (it != m_locationDispatch.end())
    {
      (this->*(it->second))(location, i, tokens);
    }
    else
    {
      std::cerr << "Unknown location directive: " << tokens[i] << std::endl;
    }
    i++;
  }
  server.locations.push_back(location);
}
