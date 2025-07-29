#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#define DEFAULT_PORT "8080"

int main(int argc, char **argv) {
    char *port;
    int status;
    struct addrinfo hints;
    struct addrinfo *serverinfo;

    // Determine port number
    if (argc > 1) {
        if (atoi(argv[1]) > 0) {
            printf("Using Port %s\n", argv[1]);
            port = argv[1];
        } else {
            printf("Invalid port %s, using default port %s\n", argv[1], DEFAULT_PORT);
            port = DEFAULT_PORT;
        }
    } else {
        printf("No port provided, using default port %s\n", DEFAULT_PORT);
        port = DEFAULT_PORT;
    }

    printf("%s", port);
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((status = getaddrinfo(NULL, port, &hints, &serverinfo)) != 0) {
        fprintf(stderr, "Error: %s\n", gai_strerror(status));
        exit(status);
    }

    freeaddrinfo(serverinfo);
}