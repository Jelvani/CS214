#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>

#define RED   "\x1B[31m"
#define GRN   "\e[1;92m"
#define BLU   "\x1b[94m"
#define RESET "\x1B[0m"



struct file{
	char* path;
	int tokCount;
	struct file* next;
	struct token* token;
	};

struct token{
	char* value;
	double prob;
	struct token* next;
};

//according to menny, we cannot have our mutex as a global, so we pass this struct to each thread argument
struct threadArg{
	char* dir;
	pthread_mutex_t* lock;
	struct file* head;
};

double calculate(struct file file1, struct file file2) {
	struct token* token1 = file1.token;
	struct token* token2 = file2.token;
	
	double file1kld = 0.0;
	double file2kld = 0.0;
	double constructedMean = 0.0;

	while(token1 != NULL) {
		char* currToken = token1->value;
		char* tok2 = NULL;
		double token2Prob = 0.0;
		while(token2 != NULL) {
			char* temp = token2->value;
			if(strcmp(currToken, temp) == 0) {
				token2Prob = token2->prob;
				tok2 = temp;
				break;
			}
			token2 = token2->next;
		}

		if(tok2 == NULL) {
			constructedMean = (token1->prob + token2Prob) / 2;
			file1kld += token1->prob * log10(token1->prob / constructedMean);
		} else {
			constructedMean = (token1->prob + token2Prob) / 2;
			file1kld += token1->prob * log10(token1->prob / constructedMean);
		}
		token2 = file2.token;
		token1 = token1->next;
	}

	token1 = file1.token;
	token2 = file2.token;

	while(token2 != NULL) {
		char* currToken = token2->value;
		char* tok1 = NULL;
		double token1Prob = 0.0;
		while(token1 != NULL) {
			char* temp = token1->value;
			if(strcmp(currToken, temp) == 0) {
				token1Prob = token1->prob;
				tok1 = temp;
				break;
			}
			token1 = token1->next;
		}

		if(tok1 == NULL) {
			constructedMean = (token2->prob + token1Prob) / 2;
			file2kld += token2->prob * log10(token2->prob / constructedMean);
		} else {
			constructedMean = (token2->prob + token1Prob) / 2;
			file2kld += token2->prob * log10(token2->prob / constructedMean);
		}
		token1 = file1.token;
		token2 = token2->next;
	}

	return (file1kld + file2kld) / 2;
}


//inserts token into shared memory structure using insertion sort
void insertToken(struct file* file, char* token){
	file->tokCount++;


	//if first token, insert and return

	if(file->token == NULL){
		//printf("FIRST: %s\n",token);
		char* tok = (char*) malloc(strlen(token) + 1);
		strcpy(tok,token);
		file->token = (struct token*) malloc(sizeof(struct token));
		file->token->next = NULL;
		file->token->value = tok;
		file->token->prob = 1;
		return;
	}
	struct token* ptr = (struct token*) malloc(sizeof(struct token));
	ptr = file->token;
	if(strcmp(ptr->value,token)==0){
		ptr->prob++;
		return;
	}
	while(ptr->next!=NULL){
		if(strcmp(ptr->value,token)==0){
			ptr->prob++;
			return;
		}
		ptr = ptr->next;
	}
	if(strcmp(ptr->value,token)==0){
		ptr->prob++;
		return;
	}

	ptr->next = (struct token*) malloc(sizeof(struct token));
	ptr->next->value = (char*) malloc(strlen(token) + 1);
	strcpy(ptr->next->value,token);
	ptr->next->prob++;

	/*
	//if token is less than first value in list
	if(strcmp(token,file->token->value) < 0){
		//printf("LESS: %s\n",token);
		char* tok = (char*) malloc(strlen(token) + 1);
		strcpy(tok,token);
		struct token* tmp = file->token;
		file->token = (struct token*) malloc(sizeof(struct token));
		file->token->next = tmp;
		file->token->value = tok;
		file->token->prob = 1;
		return;
	}

	// all other cases, insertion sort
	struct token* tmp = file->token;
	struct token* prev = tmp;
	
	while(tmp->next!=NULL){
		if(strcmp(token,tmp->value) < 0){
			//printf("NEW: %s\n",token);
			char* tok = (char*) malloc(strlen(token) + 1);
			strcpy(tok,token);
			prev->next  = (struct token*) malloc(sizeof(struct token));
			prev->next->value = tok;
			prev->next->next = tmp;
			prev->next->prob = 1;
			return;
		}else if(strcmp(token,tmp->value) == 0){
			//printf("EXISTS: %s\n",token);
			tmp->prob++;
			return;
		}
		prev = tmp;
		tmp=tmp->next;
	}
	tmp->next = (struct token*) malloc(sizeof(struct token));
	tmp->next->next=NULL;
	tmp->next->value = (char*) malloc(strlen(token) + 1);
	strcpy(tmp->next->value,token);
	tmp->next->prob = 1;
	*/

}

void* fileHandle(void* directory){
	struct threadArg* args = (struct threadArg*) directory;
	printf("FILE HANDLE: %s\n",args->dir);
	//attempt to open file
	int fd;
	if((fd = open(args->dir,O_RDONLY)) == -1){
		printf("ERR: cannot open file %s\n",args->dir);
	}else{
		//set shared data structure head to end of LL
		pthread_mutex_lock(args->lock);
		struct file* tmp = args->head;
		while(tmp->next!=NULL){
			tmp = tmp->next;
		}	
		//allocate new file node
		tmp->next = (struct file*) malloc(sizeof(struct file));
		tmp->next->next = NULL;
		tmp->next->token = NULL;
		tmp->next->tokCount = 0;
		pthread_mutex_unlock(args->lock);
		//copy file path/name to file node
		tmp->next->path = (char*) malloc(strlen(args->dir)+1);
		strcpy(tmp->next->path, args->dir);
		
		//start reading data into buffer of size 256, then copy to larger array.
		int rs = 0;
		char buf;
		int tokbuf = 256;//initial maximum token size
		char* token = (char*) malloc(tokbuf);
		int ind = 0;//index of current token

		//this loop will read characters 1 by 1 from file and when reaches a whitespace, send token to be inserted. then repeat until EOF
		while((rs = read(fd,&buf,1)) > 0){
			if(isalpha(buf)==0 && isspace(buf)==0 && (buf!= '-')){//if char is not alphabetic or a whitespace and not a hyphen then skip it
				continue;	
			}
			//when we encounter a whitespace,add terminator and send token to insertToken()
			if(isspace(buf)){
				token[ind] = '\0';
				ind = 0;
				insertToken(tmp->next,token);
				continue;
			}
			buf = tolower(buf);
			token[ind] = buf;
			ind++;

			//if we reach end of token buffer, realloc with double the size.
			if(ind>tokbuf-1){
				tokbuf = tokbuf*2;
				token = (char*) realloc(token, tokbuf);
			}
		}

		

		//after done inserting, compute discrete probabilities
		struct token* curtok = tmp->next->token;
		while(curtok!=NULL){
			curtok->prob = (curtok->prob) / tmp->next->tokCount;
			curtok = curtok->next;
		}



	}

	close(fd);
	free(args->dir);
	free(args);
	return 0;
}
void* findDir(void* directory) {
	struct threadArg* args = (struct threadArg*) directory;
    DIR *dir;
    struct dirent *dp;
	pthread_t* thread_list = (pthread_t*) malloc(sizeof(pthread_t));//array of thread id's
	int trdCnt = 1;
    //checks to see if can open directory, if not fail it.
	
    if((dir = opendir(args->dir)) == NULL) {
		printf("ERR: cannot open %s\n",args->dir);
		printf(RESET);
		exit(1);
	}

	printf(RESET);
    while((dp = readdir(dir)) != NULL) { 

        if(dp->d_type == DT_DIR) {
			//check for previous and current dir links
            if(strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) {
                continue;
            }
			//create argument structure and spawn new directory thread
			struct threadArg* new_args = (struct threadArg*) malloc(sizeof(struct threadArg));
            new_args->dir = (char*) malloc(strlen(args->dir) + strlen(dp->d_name) + 2);
			strcpy(new_args->dir,args->dir);
            strcat(new_args->dir, "/");
            strcat(new_args->dir, dp->d_name);
			new_args->lock = args->lock;
			new_args->head = args->head;
			printf(BLU "Found dir: %s\n", new_args->dir);
			pthread_create(thread_list+trdCnt-1,NULL,findDir,(void*) new_args);
			trdCnt++;
			//dynamicly resize array
			thread_list = (pthread_t*) realloc(thread_list,sizeof(pthread_t)*trdCnt);
		
        } else if(dp->d_type == DT_REG) {//found a regular file
			//check for exectuable in file handler thread
			struct threadArg* new_args = (struct threadArg*) malloc(sizeof(struct threadArg));
            new_args->dir = (char*) malloc(strlen(args->dir) + strlen(dp->d_name) + 2);
			strcpy(new_args->dir,args->dir);
            strcat(new_args->dir, "/");
            strcat(new_args->dir, dp->d_name);
			new_args->lock = args->lock;
			new_args->head = args->head;
			printf(GRN "Found file: %s\n", dp->d_name);
			pthread_create(thread_list+trdCnt-1,NULL,fileHandle,(void*) new_args);
			trdCnt++;
			//dynamicly resize array
			thread_list = (pthread_t*) realloc(thread_list,sizeof(pthread_t)*trdCnt);
        }
        printf(RESET);
    }
    closedir(dir);
	for(int i=0; i<trdCnt-1; i++){
		pthread_join(thread_list[i],NULL);
	}
    
	free(args->dir);
	free(thread_list);
	free(args);
	return 0;
}

int main(int argc, char *argv[]) {
    //Checks for args count
    if(argc != 2){
        printf(RESET);
        printf("ERR: INVALID ARG COUNT\nTo properly run the program, enter a directory to start scanning.\n");
        return EXIT_FAILURE;
    }

    DIR *dir;
	
	struct threadArg* args = (struct threadArg*) malloc(sizeof(struct threadArg));
	args->lock = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
        //initializes mutex lock for synch
        if(pthread_mutex_init(args->lock, NULL) != 0) { 
            printf("mutex init has failed\n"); 
            return 1; 
        } 

    char* fullpath;
	
	if((fullpath=realpath(argv[1], NULL))==NULL){
		printf(RED "The path: '%s' is invalid!\n",argv[1]);
		printf(RESET);
		return EXIT_FAILURE;
	}
	
	args->dir = fullpath;
	args->head = (struct file*) malloc(sizeof(struct file));
		//does not need to be thread as per instruction (1.c)
	
    findDir((void*) args);
	

//sample loop going through all files and tokens in our shared data structure	
	while(args->head!=NULL){
		printf("FILE: %s\n", args->head->path);
		while(args->head->token!=NULL){
			printf("TOKEN: %s WITH PROB: %f\n",args->head->token->value,args->head->token->prob);
			args->head->token = args->head->token->next;
		}
		args->head = args->head->next;
	}
	
	
	/*struct file file1;
	file1.token = malloc(sizeof(struct token));
	file1.tokCount = 4;
	file1.token->value = "hi";
	file1.token->prob = 0.5;
	file1.token->next = malloc(sizeof(struct token));
	file1.token->next->value = "there";
	file1.token->next->prob = 0.5;
	file1.token->next->next = NULL;

	//c.txt, hi = prob 0.5, out = prob 0.25, there = prob 0.25
	struct file file2;
	file2.token = malloc(sizeof(struct token));
	file2.tokCount = 4;
	file2.token->value = "hi";
	file2.token->prob = 0.5;
	file2.token->next = malloc(sizeof(struct token));
	file2.token->next->value = "out";
	file2.token->next->prob = 0.25;
	file2.token->next->next = malloc(sizeof(struct token));
	file2.token->next->next->value = "there";
	file2.token->next->next->prob = 0.25;
	file2.token->next->next->next = NULL;
	double result = calculate(file1, file2);
	printf("Result for file1 and file2 is: %f\n", result);*/

    printf(RESET);
    return EXIT_SUCCESS;
}
