#include "webserver.hpp"
 
std::string search_find(const std::string &word, std::vector<char> &stc, ssize_t *bytes_read)
{
  std::vector<char>::const_iterator end_it = stc.begin() + *bytes_read;
  std::vector<char>::const_iterator it = std::search(stc.begin(), end_it, word.begin(), word.end());
  if (it != end_it)
  {
    std::vector<char>::const_iterator start = it;
    while (start != stc.begin() && *start != ' ')
      start--;

    if(*start = ' ') start++;
    std::vector<char>::const_iterator end = it;
    while (end != end_it && *end != ' ' 
          && *end != '\r' && *end != '\n')
    {end++;}

    return std::string(start, end);
  }

  return "";

}


int main(void)
{
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in server_add;
  struct sockaddr_in client_add;
  
  setuping(&server_fd, &server_add);
  int new_socket = setuping_recv(&server_fd, (struct sockaddr *)&client_add);
   
  std::vector<char> buff(1024);
  ssize_t bytes_read = recv(new_socket, buff.data(), buff.size(), 0);
  std::cout << "Received request" << std::endl;

  
  printing_vect(&buff, &bytes_read);

  std::string pattern = ".html"; 
  /*std::vector<char>::iterator it = std::search(buff.begin(), buff.end(), pattern, pattern + pattern_len);

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
  }*/ 
  
  std::vector<char> file(1024);
  std::string http_response =  search_find(pattern, file, &bytes_read);
  printing_vect(std::vector<char> &file);
  
  ssize_t dataSent = send(new_socket, http_response.c_str(), http_response.size(),0);
  if (dataSent < 0)
  {
    std::cerr << "Error in the sent" << std::endl;
    close(server_fd);
    close(new_socket);
    return -1;
  }
  if (static_cast<size_t>(dataSent) == http_response.size()) {std::cout << "We cool"<< std::endl;}
  else {std::cout << "Only " << dataSent << "was sent over " << http_response.size() << std::endl;}
  close(new_socket);

  close(server_fd);
  return 0;
}
