#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("ERR: INVALID ARG COUNT\nTo properly run the program, enter a port in valid range to start server.\n");
        return EXIT_FAILURE;
    }

    //convert argument to string using atoi
    int port = atoi(argv[1]);
    struct sockaddr_in server_addr, client_addr;

    if(port < 5000 || port > 65536) {
        printf("ERR: port is in invalid range! Please use a port within range from 5000-65536.\n");
        return EXIT_FAILURE;
    }

    //creates server socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (struct sockaddr*) &server_addr, sizeof(server_addr));

    listen(server_socket, 5);

    int client_socket = accept(server_socket, (struct sockaddr*) &client_addr, (socklen_t*) sizeof(client_addr));
    //close(server_socket);

    return EXIT_SUCCESS;
}