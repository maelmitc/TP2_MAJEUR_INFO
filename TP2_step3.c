#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h> 
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    // Verification of the number of arguments
    if (argc != 3) {
        fprintf(stderr, "Required form: %s <host> <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    //Extraction of request arguments
    char *host = argv[1];
    char *file = argv[2];

    // Display of arguments
    printf("Host: %s\n", host);
    printf("File: %s\n", file);

    // Creating a UDP socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct addrinfo hints, *result, *p;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC; // AF_UNSPEC allows both IPv4 and IPv6
    hints.ai_socktype = SOCK_DGRAM; // Use SOCK_DGRAM for UDP

    int status = getaddrinfo(host, NULL, &hints, &result);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        close(sockfd); // Close the socket before exiting
        exit(EXIT_FAILURE);
    }

    // Iterate through the list of addresses and print information
    for (p = result; p != NULL; p = p->ai_next) {
        char addr[INET6_ADDRSTRLEN];

        // Print the IP address
        void *ipaddr;
        if (p->ai_family == AF_INET) {
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            ipaddr = &(ipv4->sin_addr);
        } else {
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            ipaddr = &(ipv6->sin6_addr);
        }

        // Convert the IP address to a human-readable form
        inet_ntop(p->ai_family, ipaddr, addr, sizeof(addr));
        printf("IP Address: %s\n", addr);
    }

    // Free the memory allocated by getaddrinfo
    freeaddrinfo(result);

    // Close the socket
    close(sockfd);

    return 0;
}
