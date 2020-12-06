#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#define KKJ_L 8






char* getKKJ(char* payload, int* length){//returns string with implemented KKJ protocl
	*length = strlen(payload) +KKJ_L+2;
	char* message = (char*) malloc(*length)+2;
	sprintf(message,"REG|%d|%s|\n",*length-KKJ_L,payload);
	//8 here signifies our required format byte length without payload: REG|##|blahblah|
	
	return message;
}
char* readMessage(int fd){//takes in socket file descriptor. parses REG message from client and returns a string of the payload. returns NULL on error
	int seen = 0;
	char header[4];
	char charLen[10];
	char byte = 0;
	char pipe = '0';
	for(int i =0;i<4;i+=read(fd,&header[i],1)){

	}
	if(strncmp(header,"REG|",4)!=0){//message already doesnt conform to REG|
		return NULL;
	}
	char temp;
	int tmp = 0;
	while(1){
		if(read(fd,&temp,1)==1){
			if(temp=='|'){
				charLen[tmp] = '\0';
				break;
			}
			charLen[tmp] = temp;
			tmp++;
		}
	}
			
	
	int len = atoi(charLen);

	if(len==0){//not 2 digits
		return NULL;
	}

	char* message = (char*) malloc(len+1);
	for(int i =0;i<len;i+=read(fd,&message[i],1)){
		if(message[i]=='|'){
			printf("INVALID MESSAGE LENGTH!\n");
			return NULL;
		}
	}
	message[len] = '\0';
	for(int i =0;i<1;i+=read(fd,&pipe,1)){

	}
	if(pipe!='|'){
		return NULL;
	}
	//if we get here, we have received a valid message, so we return the message
	return message;
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
		int len = 0;
		char* string = getKKJ("Knock, knock.",&len);
		write(fd_client,string,len);
		char* m1 = readMessage(fd_client);
		if(m1!=NULL){
			printf("%s\n",m1);
		}
	

		close(fd_client);
		break;

	}

	close(fd_sock);
    
    return EXIT_SUCCESS;
}
