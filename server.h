#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>

#define BUFFER_SIZE 1024

struct HTTPS_Server {
    int domain;
    int port;
    int service;
    int protocol;
    int backlog;
    u_long interface;
    int socket;
    struct sockaddr_in address;
    void (*launch)(struct HTTPS_Server* server);
};

struct HTTPS_Server server_init(int domain, int port, int service, int protocol, 
    int backlog, u_long interface, void (*launch)(struct HTTPS_Server* server));

void launch(struct HTTPS_Server* server);

#endif // SERVER_H