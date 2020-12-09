#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ctype.h>
#include <math.h>
#define KKJ_L 6

enum em{//error messgae enum
	m0ct,
	m0ln,
	m0ft,
	m1ct,
	m1ln,
	m1ft,
	m2ct,
	m2ln,
	m2ft,
	m3ct,
	m3ln,
	m3ft,
	m4ct,
	m4ln,
	m4ft,
	m5ct,
	m5ln,
	m5ft
};

char* getKKJ(char* payload, int* length){//returns string with implemented KKJ protocl
	int tmp = strlen(payload);
	tmp = floor(log10((double) tmp))+1;//length of chars for message length
	*length = strlen(payload) +KKJ_L+tmp;
	char* message = (char*) malloc(*length)+1;
	sprintf(message,"REG|%d|%s|",*length-KKJ_L,payload);
	//8 here signifies our required format byte length without payload: REG|##|blahblah|
	
	return message;
}

void sendError(int fd, enum em type){//take in error type, and returns length and string pointer
	int length = 9;
	char* message = (char*) malloc(length);
	char payload[5];
	//not best way, oh well. easier for other end to keep track of
	switch (type){
		case m1ct:
			strcpy(payload,"M1CT");
			break;
		case m1ln:
			strcpy(payload,"M1LN");
			break;
		case m1ft:
			strcpy(payload,"M1FT");
			break;
		case m3ct:
			strcpy(payload,"M3CT");
			break;
		case m3ln:
			strcpy(payload,"M3LN");
			break;
		case m3ft:
			strcpy(payload,"M3FT");
			break;
		case m5ct:
			strcpy(payload,"M5CT");
			break;
		case m5ln:
			strcpy(payload,"M5LN");
			break;
		case m5ft:
			strcpy(payload,"M5FT");
			break;
	}
	sprintf(message,"ERR|%s|",payload);
	write(fd,message,length);
}
void getError(char* message){//checks if client sent back error message. takes in error message (ex. M1CT), prints description of error to stdout
	//implement
}

int readMessage(int fd,char* message){//takes in socket file descriptor. parses REG message from client and puts message in 'message' char*. returns 0 on success.
	int seen = 0;
	char header[4];
	char charLen[10];//length 10 buffer for message size
	char byte = 0;
	char pipe = '0';
	int flagErr = 0;
	int len = 0;
	for(int i =0;i<4;i++){
		read(fd,&header[i],1);
	}

	if(strncmp(header,"ERR|",4)==0){//Error message
		flagErr=1;
	}else if(strncmp(header,"REG|",4)!=0){//message already doesnt conform to REG|
		printf("no REG| found!\n");
		return -1;
	}
	if(flagErr==0){
		char temp;
		int tmp = 0;
		for(int i=0;i<10;i++){
			if(read(fd,&temp,1)==0){
				printf("Connection closed!\n");
				return -1;
			}
			charLen[i] = temp;
			if(charLen[i]=='|'){
				charLen[i]='\0';
				break;
			}
		}

		len = atoi(charLen);
		if(len==0){//not valid message length
			printf("Null message lenth!\n");
			return -1;
		}
	}else{
		len = 4;//set length to error message size
	}
	message = (char*) malloc(len+1);
	for(int i =0;i<len;i++){
		if(read(fd,&message[i],1)==0){
			printf("Connection closed!\n");
			return -1;

		}
		if(message[i]=='|'){
			printf("INVALID MESSAGE LENGTH!\n");
			return -1;
		}
	}
	message[len] = '\0';
	

	read(fd,&pipe,1);

	if(pipe!='|'){
		printf("Missing closing pipe!\n");
		return -1;
	}
	//if we get here, we have received a valid message, so we return success
	if(flagErr==0){
		return 0;
	}else{
		return -1;
	}
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
		//<Knock, knock
		char* string = getKKJ("Knock, knock.",&len);
		printf("%s\n",string);
		write(fd_client,string,len);
		//<<Who's there?
		char* m1;

		if(readMessage(fd_client,m1)!=0){
			getError(m1);
			close(fd_client);
			continue;
		}
		
		if(strncmp(m1,"Who's there?",12)!=0){
			printf("Error M1CT!\n");
			continue;
		}
		//<Who.
		string = getKKJ("Who.",&len);
		printf("%s\n",string);
		write(fd_client,string,len);
		//<<Who, who?

		if(readMessage(fd_client,m1)!=0){
			getError(m1);
			close(fd_client);
			continue;
		}

		if(strncmp(m1,"Who, who?",9)!=0){
			sendError(fd_client,m3ct);
			close(fd_client);
			continue;		}
		//<I didn't know you were an owl!
		string = getKKJ("I didn't know you were an owl!",&len);
		printf("%s\n",string);
		write(fd_client,string,len);
		//<<Ugh.
		if(readMessage(fd_client,m1)!=0){
			getError(m1);
			close(fd_client);
			continue;
		}

		if(ispunct(m1[strlen(m1)-1])==0){//if not a puctuation as last character, error
			sendError(fd_client,m5ct);
		}

		close(fd_client);

	}

	close(fd_sock);
    
    return EXIT_SUCCESS;
}
