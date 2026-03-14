#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#define PORT 8080

#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <cstring>
#include <netinet/in.h>
#include <string>
#include <vector>
#include <algorithm> 
#include <fstream>
#include <iterator>

int setuping(int *socket, struct sockaddr_in *server_add);
int setuping_recv(int *socket, struct sockaddr *client_addr);
void printing_vect(const std::vector<char> &buff, ssize_t bytes_read);

std::string search_find(const std::string &word, const std::vector<char> &stc, ssize_t *bytes_read);
std::string sending(std::string &index);
std::string get_content(const std::string& filename);
#endif 
