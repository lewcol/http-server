#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define DEFAULT_PORT "8080"
#define MAX_CONN SOMAXCONN

int main(int argc, char **argv) {
    char *port;
    int status;
    int socketfd;
    struct addrinfo hints;
    struct addrinfo *serverinfo;

    // Determine port number
    // It may be necessary to handle well-known ports differently
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

    // Assign values to hints struct
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    // Call getaddrinfo and receive addrinfo
    printf("Requesting address info\n");
    if ((status = getaddrinfo(NULL, port, &hints, &serverinfo)) != 0) {
        fprintf(stderr, "Error: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    // Get socket file descriptor
    printf("Requesting socket file descriptor\n");
    if ((socketfd = socket(serverinfo->ai_family, serverinfo->ai_socktype, serverinfo->ai_protocol)) < 0) {
        perror("Error getting socket file descriptor");
        freeaddrinfo(serverinfo);
        exit(EXIT_FAILURE);
    }

    // Reuse port if already in use
    int y = 1;
    setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &y, sizeof y);

    // Bind to the given port
    printf("Binding socket to port %s\n", port);
    if ((bind(socketfd, serverinfo->ai_addr, serverinfo->ai_addrlen)) != 0) {
        fprintf(stderr, "Error binding to port %s: %s\n", port, strerror(errno));
        close(socketfd);
        freeaddrinfo(serverinfo);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    printf("Preparing to listen on port %s\n", port);
    if ((listen(socketfd, MAX_CONN)) != 0) {
        fprintf(stderr, "Error preparing to listen on port %s: %s\n", port, strerror(errno));
        close(socketfd);
        freeaddrinfo(serverinfo);
        exit(EXIT_FAILURE);
    }

    char ip[INET_ADDRSTRLEN];
    inet_ntop(serverinfo->ai_family, serverinfo->ai_addr, ip, INET_ADDRSTRLEN);
    printf("Listening on %s:%s (max %s)\n", ip, port, MAX_CONN);

    // Close socket for program exit
    printf("Closing socket file descriptor");
    close(socketfd);

    // Free addrinfo after use
    printf("Freeing address info\n");
    freeaddrinfo(serverinfo);

    return 0;
}