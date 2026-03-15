#include "webserver.hpp"
 

int main(void)
{
  Server server;
  try
  {
    server.run();
  }
  catch(const std::exception& e)
  {
    std::cout << e.what();
    return 1;
  }

  return 0;
}
