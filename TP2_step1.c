#include <stdio.h>
#include <stdlib.h>

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


    return 0;
}
