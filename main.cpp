#include "webserver.hpp"
 



int main(void)
{
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in server_add;
  struct sockaddr_in client_add;
  
  setuping(&server_fd, &server_add);
  int new_socket = setuping_recv(&server_fd, (struct sockaddr *)&client_add);
   
  std::vector<char> buff(1024);
  recv(new_socket, buff.data(), buff.size(), 0);
  std::cout << "Received reauest" << std::endl;

  std::string html_body = "<html><body><h1>Hello from my C++ Server!</h1></body></html>";
  
  for (std::vector<char>::const_iterator i = buff.begin(); i != buff.end(); ++i)
    std::cout << *i;
  
  std::string http_response = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: " + std::to_string(html_body.length()) + "\r\n"
        "Connection: close\r\n"
        "\r\n" + html_body;

  const char pattern[] = ".html";
  const size_t pattern_len = std::strlen(pattern);
  
  std::vector<char>::iterator it = std::search(buff.begin(), buff.end(), pattern, pattern + pattern_len);

  if (it != buff.end())
  {
    std::cout << "Got it " << std::endl;
  }

  while(*it != ' ')
  {
    it--;
  }

  std::vector<char> file;
  it++;
  while(*it != ' ')
  {
    file.push_back(*it);
    it++;
  }

  for (std::vector<char>::const_iterator i = file.begin(); i != file.end(); ++i)
    std::cout << *i;
  std::cout << std::endl;
  int dataSent = send(new_socket, http_response.c_str(), http_response.size(),0);
  if (dataSent < 0)
  {
    std::cerr << "Error in the sent" << std::endl;
    close(server_fd);
    close(new_socket);
    return -1;
  }
  if (static_cast<long unsigned int>(dataSent) == http_response.size()) {std::cout << "We cool"<< std::endl;}
  else {std::cout << "Only " << dataSent << "was sent over " << http_response.size() << std::endl;}
  close(server_fd);
  close(new_socket);
  return 0;
}
