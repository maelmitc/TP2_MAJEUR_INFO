#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main(int argc, char *argv[]) {
    // Verification of the number of arguments
    if (argc != 3) {
        fprintf(stderr, "Required_form: %s <host> <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Extraction of request arguments
    char *host = argv[1];
    char *file = argv[2];

    // Display of arguments
    printf("Host: %s\n", host);
    printf("File: %s\n", file);

    struct addrinfo hints, *result;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC; // AF_UNSPEC allows both IPv4 and IPv6
    hints.ai_socktype = SOCK_DGRAM; // Use SOCK_DGRAM for UDP

    int status = getaddrinfo(host, NULL, &hints, &result);
    if (status != 0) {
        fprintf(stderr, "Error resolving host: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    // Display the resolved IP addresses
    struct addrinfo *p;
    for (p = result; p != NULL; p = p->ai_next) {
        void *addr;
        char ipstr[INET6_ADDRSTRLEN];

        if (p->ai_family == AF_INET) {
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
        } else {
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
        }

        // Convert the IP address to a human-readable string
        inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
        printf("Resolved IP: %s\n", ipstr);
    }

    freeaddrinfo(result); // Free the memory allocated by getaddrinfo

    return 0;
}
