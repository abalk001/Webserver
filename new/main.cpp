#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include "ConfigParser.hpp"

// this fct is working perfectly !!






int main(void)
{
  ConfigParser conf;
  std::vector<std::string> tokens;
  /*tokens = 
  std::vector<std::string>::iterator end = tokens.end();
  for (std::vector<std::string>::iterator it = tokens.begin(); it < end; ++it)
  {
    std::cout << *it << std::endl;
  }*/
  conf.parsefile("default.conf");
  return 0;
}
