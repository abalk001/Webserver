#include "webserver.hpp"
 

int main(void)
{
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in server_add;
  struct sockaddr_in client_add;
  
  setuping(&server_fd, &server_add);
  while (1)
  {
  int new_socket = setuping_recv(&server_fd, (struct sockaddr *)&client_add);
   
  std::vector<char> buff(1024);
  ssize_t bytes_read = recv(new_socket, buff.data(), buff.size(), 0);
  std::cout << "Received request" << std::endl;

  
  printing_vect(buff, bytes_read);

  std::string pattern = ".html";
  std::string index_html =  search_find(pattern, buff, &bytes_read);
  if (index_html.size() == 0)
  {
    pattern = ".css";
    index_html = search_find(pattern,buff, &bytes_read);
  }

  std::cout << index_html << std::endl;
  std::string http_response = sending(index_html);
  
  ssize_t dataSent = send(new_socket, http_response.c_str(), http_response.size(),0);
  if (dataSent < 0)
  {
    std::cerr << "Error in the sent" << std::endl;
    close(server_fd);
    close(new_socket);
    return -1;
  }
  if (static_cast<size_t>(dataSent) == http_response.size())
    std::cout << "We cool"<< std::endl;
  else 
    std::cout << "Only " << dataSent << "was sent over " << http_response.size() << std::endl;
  close(new_socket);}
  close(server_fd);
  return 0;
}
