#include <sys/socket.h>
#include <cstring>
#include <unistd.h>
#include <netdb.h>
#include <iostream>
#include <string>
#include <vector>


# define PORT 8080 // port to where the client is connected 

int main()
{
  int client_fd = socket(AF_INET, SOCK_STREAM, 0);
  std::string where_to_connect = "127.0.0.1";
  struct hostent *he;
  if ((he = gethostbyname(where_to_connect.c_str())) == NULL)
  {
    std::cerr << "error here in the gethostbyname" << std::endl;
    close(client_fd);
    return -1;
  }
  struct sockaddr_in client_add;
  memset(&client_add, 0, sizeof(client_add));

  client_add.sin_family = AF_INET;
  client_add.sin_port = htons(PORT);
  client_add.sin_addr = *((struct in_addr *)he->h_addr);
  if (connect(client_fd, (struct sockaddr *)&client_add, sizeof(struct sockaddr)) == -1)
  {
    std::cerr << "there is an issue here "<< std::endl;
    close(client_fd);
    return -1;
  }
  std::vector<char> buf(5000);
  ssize_t nbr = recv(client_fd, buf.data(), buf.size(), 0);
  std::cout << nbr << std::endl;

  close(client_fd);
  return 0;
}
