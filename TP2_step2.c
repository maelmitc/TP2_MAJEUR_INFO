#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


int getaddrinfo(const char *node, const char *service,
const struct addrinfo *hints, struct addrinfo **res);
struct addrinfo {
int ai_flags;
int ai_family;
int ai_socktype;
int ai_protocol;
socklen_t ai_addrlen;
struct sockaddr *ai_addr;
char *ai_canonname;
struct addrinfo *ai_next;
};

int main(int argc, char *argv[]) {
    // Verification of the number of arguments
    if (argc != 3) {
        fprintf(stderr, "Required_form: %s <host> <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    //Extraction of request arguments
    char *host = argv[1];
    char *file = argv[2];
    
    // Display of arguments
    printf("Host: %s\n", host);
    printf("File: %s\n", file);

    struct addrinfo hints, *result;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC; // AF_UNSPEC allows both IPv4 and IPv6
    hints.ai_socktype = SOCK_DGRAM; // Use SOCK_STREAM for TCP

    int status = getaddrinfo(host, NULL, &hints, &result);
    char addr = result->ai_addr;
    printf(addr);

    return 0;
}
