#include "webserver.hpp"
 

int main(void)
{
  Server server;
  int error;

  error = server.setuping();
  if (error == -1)
    return error; 
  server.setuping_recv();
  ssize_t  bytes_read = server.receiving();
  if (bytes_read < 0) // there is leaks here, need to fix that 
  {
    std::cout <<"Error"<< std::endl;
    return -1;
  }

  server.sending(bytes_read);  
  return 0;
}
