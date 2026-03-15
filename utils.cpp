#include "webserver.hpp"
#include <string>

bool fileExists(const std::string& filename)
{
  std::ifstream file(filename.c_str());
  return file.good();
}

std::string sendingI(const std::string &path)
{
  std::string html_body;
  std::string status = "HTTP/1.1 200 OK\r\n";
  std::string content_type = get_content(path);

  if (fileExists(path))
  {
    std::ifstream file(path.c_str(), std::ios::binary);
    if (file.is_open())
    {
      html_body.assign((std::istreambuf_iterator<char>(file)),
          std::istreambuf_iterator<char>());
      file.close();
    }
    else
    {
      status = "HTTP/1.1 500 Internal Server Error\r\n";
      html_body = "<html><body><h1>500 Internal Server Error</h1></body></html>";
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
  if (ex == ".js")
    return "application/javascript";

  return "text/plain";
}

