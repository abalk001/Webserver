#include "webserver.hpp"




int setuping(int *socket, struct sockaddr_in  *server_add)
{

  int y = 1;
  
  memset(server_add, 0, sizeof(&server_add));
  server_add->sin_family = AF_INET;
  server_add->sin_port = htons(PORT);
  server_add->sin_addr.s_addr = INADDR_ANY;

  if (setsockopt(*socket, SOL_SOCKET, SO_REUSEADDR, &y, sizeof(int)) == -1)
  {
    std::cerr << "error in the setsockopt"<< std::endl;
    return -1;
  }
  if (bind(*socket, (struct sockaddr *)server_add, sizeof(struct sockaddr)))
  {
    std::cerr << "Error in the binding" << std::endl;
    return -1;
  }
  
  if (listen(*socket, 5) < 0)
  {
    std::cerr << "error in the listening part" << std::endl;
    return -1;
  }
  std::cout << "The sever is listening" << std::endl;

  return 1;
}

int setuping_recv(int *socket, struct sockaddr *client_addr)
{
  socklen_t sin_size = sizeof(*client_addr);
  int new_socket = accept(*socket, client_addr, &sin_size);
  if (new_socket < 0)
  {
    std::cerr << "There is an issue with accept" << std::endl;
    return -1;
  }

  return new_socket;
}
