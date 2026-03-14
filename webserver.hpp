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


int setuping(int *socket, struct sockaddr_in *server_add);
int setuping_recv(int *socket, struct sockaddr *client_addr);
void printing_vect(const std::vector<char> &buff, ssize_t bytes_read);
#endif 
