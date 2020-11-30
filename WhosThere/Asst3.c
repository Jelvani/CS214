#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    if(argc != 2) {
        printf("ERR: INVALID ARG COUNT\nTo properly run the program, enter a port in valid range to start server.\n");
        return EXIT_FAILURE;
    }

    //convert argument to string using atoi
    int port = atoi(argv[1]);

    if(port < 5000 || port > 65536) {
        printf("ERR: port is in invalid range! Please use a poor from 5000-65536.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}