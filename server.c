#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "server.h"

struct HTTPS_Server server_init(int domain, int port, int service, int protocol, int backlog, u_long interface,
    void (*launch)(struct HTTPS_Server* server)) {
        struct HTTPS_Server server;
        server.domain = domain;
        server.port = port;
        server.service = service;
        server.protocol = protocol;
        server.backlog = backlog;
        server.address.sin_family = domain;
        server.address.sin_addr.s_addr = htonl(interface);
        server.address.sin_port = htons(port);

        server.socket = socket(domain, service, protocol);
        if (server.socket < 0) {
            perror("Failed to initialize socket\n");
            exit(1);
        }

        if (bind(server.socket, (struct sockaddr*)&server.address, sizeof(server.address)) < 0) {
            perror("Failed to bind socket\n");
            exit(1);
        }

        if (listen(server.socket, server.backlog) < 0) {
            perror("Failed to start listening\n");
            exit(1);
        }

        server.launch = launch;
        return server;
}

void launch(struct HTTPS_Server* server) {
    char buffer[BUFFER_SIZE];
    while(1) {
        printf("--- AWAITING CONNECTION ---\n");
        int addrlen = sizeof(server->address);
        int new_socket = accept(server->socket, (struct sockaddr*)&server->address, (socklen_t*)&addrlen);
        ssize_t bytes_read = read(new_socket, buffer, BUFFER_SIZE - 1);

        if (bytes_read >= 0) {
            buffer[bytes_read] = '\0';
            puts(buffer);
        } else {
            perror("Error reading buffer\n");
        }

        char *response = "HTTP/1.1 200 OK\r\n"
                         "Content-Type: text/html; charset=UTF-8\r\n\r\n"
                         "<!DOCTYPE html>\r\n"
                         "<html>\r\n"
                         "<head>\r\n"
                         "<title>Testing Server</title>\r\n"
                         "</head>\r\n"
                         "<body>\r\n"
                         "Hello!\r\n"
                         "</body>\r\n"
                         "</html>\r\n";
        write(new_socket, response, strlen(response));
        close(new_socket);
    }
}