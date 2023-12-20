#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main(int argc, char *argv[]) {
    // Verification of the number of arguments
    if (argc != 3) {
        fprintf(stderr, "Required form: %s <host> <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Extraction of request arguments
    char *host = argv[1];
    char *file = argv[2];

    // Display of arguments
    printf("Host: %s\n", host);
    printf("File: %s\n", file);

    // Creation of the socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // Error test
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct addrinfo hints, *result;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC; // AF_UNSPEC allows both IPv4 and IPv6
    hints.ai_socktype = SOCK_DGRAM; // Use SOCK_DGRAM for UDP

    int status = getaddrinfo(host, "1069", &hints, &result);
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

    // Construct RRQ packet
    char rrqPacket[516]; // Maximum size of a TFTP packet
    rrqPacket[0] = 0x00; // Opcode for RRQ (Read Request)
    rrqPacket[1] = 0x01;
    strcpy(rrqPacket + 2, file);
    rrqPacket[2 + strlen(file)] = '\0';
    strcpy(rrqPacket + 2 + strlen(file) + 1, "octet");
    rrqPacket[2 + strlen(file) + 1 + strlen("octet")] = '\0';

    // Send RRQ packet to the server
    int sdt=sendto(sockfd, rrqPacket, 2 + strlen(file) + 1 + strlen("octet") + 1, 0,
               result->ai_addr, result->ai_addrlen);
        if (sdt == -1){     
        perror("Error sending RRQ packet");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Free the memory allocated by getaddrinfo
    freeaddrinfo(result);

    return 0;
}
