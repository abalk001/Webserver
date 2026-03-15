#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>


std::vector<std::string> fct(const std::string& filename)
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
} // this fct is working perfectly !!


bool parsefile(std::string& fileame)
{
  // check if it can open the file or not 

  std::vector<std::string> tokens;
  tokens = fct(fileame);
  if(tokens.empty())
    return false;
  for (size_t i = 0; i < tokens.size(); i++)
  {
    if (tokens[i] == "server")
    {
      // we skip the server and the {  
      i += 2;
      while(i < tokens.size() && tokens[i] != "}")
      {
        if (tokens[i] == "listen")
        {
          i++;
          // add the port number port = std::atoi(tokens[i].c_str())
          i++;
          if (tokens[i] != ";")
            exit(-1); // need to handle that later
        }
        if (tokens[i] == "location")
        {
          i++; 
          // and we continue the logic for the others 
        }
      }
    }
  }
}


int main(void)
{
  std::vector<std::string> tokens;
  tokens = fct("default.conf");
  std::vector<std::string>::iterator end = tokens.end();
  for (std::vector<std::string>::iterator it = tokens.begin(); it < end; ++it)
  {
    std::cout << *it << std::endl;
  }
  return 0;
}
