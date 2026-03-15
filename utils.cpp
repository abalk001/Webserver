#include "webserver.hpp"
#include <string>

void printing_vect(const std::vector<char> &buff, ssize_t bytes_read)
{
 for (ssize_t i = 0; i < bytes_read; i++)
   std::cout << buff[i];
 std::cout << std::endl;
}

bool fileExists(const std::string& filename)
{
  std::ifstream file(filename.c_str());
  return file.good();
}

std::string sendingI(std::string &index)
{

  std::string html_body ;
  index.erase(0,1);

  std::string content_type = get_content(index);
  std::string status = "HTTP/1.1 200 OK\r\n";

  if (fileExists(index))
  {
    std::ifstream file(index.c_str(), std::ios::binary);
    if (file.is_open()) 
    {
      std::cout << "We here " << std::endl;
      html_body.assign((std::istreambuf_iterator<char>(file)), 
          std::istreambuf_iterator<char>());
      
    file.close();
    }
    else 
    {
    std::cerr << "There is an issue" << std::endl;
      exit(-1);
    }
  }
  else 
  {
    status = "HTTP/1.1 404 Not Found\r\n";
    content_type = "text/html"; 
    html_body = "<html><body><h1>ERROR 404 FILE NOT FOUND </h1></body></html>";
  }
    std::string http_response = 
        status +
        "Content-Type: " + content_type + "\r\n" +
        "Content-Length: " + to_string(html_body.length()) + "\r\n"
        "Connection: close\r\n"
        "\r\n" + html_body;
  return http_response;
}

std::string get_content(const std::string& filename)
{
  size_t dot = filename.find_last_of(".");

  if (dot == std::string::npos) 
    return "text/plain";
  std::string ex = filename.substr(dot);
  if (ex == ".html" || ex == ".htm")
    return "text/html";
  if (ex == ".css")
    return "text/css";

  return "text/plain";
}

