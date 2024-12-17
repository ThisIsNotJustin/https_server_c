#include "server.h"

int main() {
    struct HTTPS_Server server = server_init(AF_INET, 80, SOCK_STREAM, 0, 10, INADDR_ANY, launch);
    server.launch(&server);
    return 0;
}