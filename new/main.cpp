#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include "ConfigParser.hpp"

// this fct is working perfectly !!






int main(void)
{
  ConfigParser conf;
  conf.parsefile("default.conf");
  return 0;
}
