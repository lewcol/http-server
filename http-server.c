#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#define DEFAULT_PORT 8080

int main(int argc, char **argv) {
    int port, status;
    struct addrinfo hints;
    struct addrinfo *serverinfo;

    // Determine port number
    if (argc > 1) {
        if ((port = atoi(argv[1])) > 0) {
            printf("Using Port %d\n", port);
        } else {
            printf("Invalid port %d, using default port %d\n", port, DEFAULT_PORT);
            port = DEFAULT_PORT;
        }
    } else {
        printf("No port provided, using default port %d\n", DEFAULT_PORT);
        port = DEFAULT_PORT;
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((status = getaddrinfo(NULL, "80", &hints, &serverinfo)) != 0) {
        fprintf(stderr, "Error: %s\n", gai_strerror(status));
        exit(status);
    }

    freeaddrinfo(serverinfo);
}