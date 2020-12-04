#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>







int getKKJ(char* payload, char* message){//gives a char* and length imlpementing payload into KKJ protocol
	int length = strlen(payload);


	//8 here signifies our required format byte length without payload: REF|##|blahblah|
	return length + 8;
}

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


	struct addrinfo hints, *addrList, *addr;
	memset(&hints,0,sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if(getaddrinfo(NULL,argv[1],&hints,&addrList) != 0){
		printf("Error getting addrinfo!\n");
		return EXIT_FAILURE;
	}



    //loop through all our configs and exit when first socket creates
    int fd_sock;
	for(addr = addrList; addr != NULL; addr = addr->ai_next){
		if((fd_sock = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol))==-1){
			continue;
		}
		//if we get to here, this means we have a socket and we will try to bind it
		if((bind(fd_sock,addr->ai_addr,addr->ai_addrlen) == 0) && (listen(fd_sock,10) == 0)){
			break;
		}
		//something failed after creating socket, so close it and try another config
		close(fd_sock);
	}
	//check if we finally have a valid socket
	if(addr==NULL){
		printf("Error creating socket!\n");
		return EXIT_FAILURE;
	}
	//free LL
	freeaddrinfo(addrList);

	/*******************************************************/
	//ABOVE WAS JUST SETUP, NOW WE HAVE A PORT TO ACCEPT
	//CONNECTIONS ON
	/*******************************************************/
	int fd_client;

	//this loops blocks until a connection is accepted and we can begin reading/writing
	while(fd_client = accept(fd_sock,NULL,NULL)){
		write(fd_client,"test\n",5);
		close(fd_client);
		break;

	}

	close(fd_sock);
    
    return EXIT_SUCCESS;
}
