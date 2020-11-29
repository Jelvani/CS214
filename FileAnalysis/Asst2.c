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
#define YEL  "\x1B[33m"
#define CYN  "\x1B[36m"
#define WHT  "\x1B[37m"
#define RESET "\x1B[0m"

struct file {
	char* path;
	int tokCount;
	struct file* next;
	struct token* token;
};

struct token {
	char* value;
	double prob;
	struct token* next;
};

//according to menny, we cannot have our mutex as a global, so we pass this struct to each thread argument
struct threadArg { 
	char* dir;
	pthread_mutex_t* lock;
	struct file* head;
};

/**
 * calculate(file1, file2):
 * 		file1: first file to compare
 * 		file2: second file to compare
 * 
 * This method grabs both token lists, and solves for the KLD/JSD for the pair. 
 */
double calculate(struct file* file1, struct file* file2) {
	struct token* token1 = file1->token;
	struct token* token2 = file2->token;
	
	double file1kld = 0.0;
	double file2kld = 0.0;
	double constructedMean = 0.0;

	/**
	 * If tokenList for file1 is not null
	 * 		Grab currentToken (file1 token)
	 * 		
	 * 		If tokenList for file2 is not null
	 * 			Grab currentToken (file2Token)
	 * 			strcmp() to see if token1 value and token2 value are the same
	 * 				if true:
	 * 					grab probability, and set tok2 to currentToken from file2
	 * 		Proceed to next token
	 * 
	 * Calculate mean based on value of tok2
	 * Grab file1KLD
	 */ 
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
		} else {
			constructedMean = (token1->prob + token2Prob) / 2;
		}
		file1kld += token1->prob * log10(token1->prob / constructedMean);
		token2 = file2->token;
		token1 = token1->next;
	}

	//reset tokenList to beginning
	token1 = file1->token;
	token2 = file2->token;

	/**
	 * If tokenList for file2 is not null
	 * 		Grab currentToken (file2 token)
	 * 		
	 * 		If tokenList for file1 is not null
	 * 			Grab currentToken (file1Token)
	 * 			strcmp() to see if token2 value and token1 value are the same
	 * 				if true:
	 * 					grab probability, and set tok2 to currentToken from file1
	 * 		Proceed to next token
	 * 
	 * Calculate mean based on value of tok1
	 * Grab file2KLD
	 */ 
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
		} else {
			constructedMean = (token2->prob + token1Prob) / 2;
		}
		file2kld += token2->prob * log10(token2->prob / constructedMean);
		token1 = file1->token;
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
	
	//if token is less than first value in list
	if(strcmp(token,file->token->value) < 0){
		struct token* prev = file->token;
		char* tok = (char*) malloc(strlen(token) + 1);
		strcpy(tok,token);
		file->token = (struct token*) malloc(sizeof(struct token));
		file->token->next = prev;
		file->token->value = tok;
		file->token->prob = 1;
		return;
	}

	//we now know file->token is not null, so assign it to a pointer
	struct token* ptr = file->token;
	struct token* prev = ptr;
	
	while(ptr!=NULL){
		//if token less than current, insert inbetween prev and current
		if(strcmp(token,ptr->value) < 0){
			char* tok = (char*) malloc(strlen(token) + 1);
			strcpy(tok,token);
			prev->next  = (struct token*) malloc(sizeof(struct token));
			prev->next->value = tok;
			prev->next->next = ptr;
			prev->next->prob = 1;
			return;
		}else if(strcmp(token,ptr->value) == 0){
			ptr->prob++;
			return;
		}
		prev = ptr;
		ptr=ptr->next;
	}
	ptr = prev;
	//compare to last node before creating new node
		if(strcmp(token,ptr->value) == 0){
			ptr->prob++;
			return;
		}

	//if token is at end of list, create new node.
	ptr->next = (struct token*) malloc(sizeof(struct token));
	ptr->next->next=NULL;
	ptr->next->value = (char*) malloc(strlen(token) + 1);
	strcpy(ptr->next->value,token);
	ptr->next->prob = 1;
	return;
}

void* fileHandle(void* directory){
	struct threadArg* args = (struct threadArg*) directory;

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
			//if char is not alphabetic or a whitespace and not a hyphen then skip it
			if(isalpha(buf)==0 && isspace(buf)==0 && (buf!= '-')){
				continue;		
			}
			
			//when we encounter a whitespace,add terminator and send token to insertToken()
			if(isalpha(buf)==0){
				if(ind==0){
					continue;
				}
				token[ind] = '\0';
				insertToken(tmp->next,token);
				ind = 0;
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
		if(isspace(buf)==0){
			insertToken(tmp->next,token);
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
			//printf(BLU "Found dir: %s\n", new_args->dir);
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
			//printf(GRN "Found file: %s\n", dp->d_name);
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

//recursvie method, returns a sorted LL when given 2
struct file* mergeLists(struct file* first, struct file* second){
	struct file* res = NULL;
	if(first==NULL){
		return second;
	}
	if(second==NULL){
		return first;
	}
	if(first->tokCount <=  second->tokCount){
		res = first;
		res->next = mergeLists(first->next,second);
	}else{
		res = second;
		res->next = mergeLists(first,second->next);
	}
	return res;
}

struct file* sortFiles(struct file* head){
	if(head == NULL || head->next == NULL){
		return head;
	}
	struct file* first = head;
	struct file* second = head;
	struct file* tail = head;
	//get middle of LL
	while(tail->next!=NULL && tail->next->next!=NULL){
		tail = tail->next->next;
		first = first->next;
	}
	//second is head of second list, first is head of curst list
	second = first->next;
	first->next = NULL;

	//call the this function again until all nodes are split
	second = sortFiles(second);
	first = sortFiles(head);
	//return sorted list that compares only first values
	return mergeLists(first,second);
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
	

	//begin populaitng datastructure
    findDir((void*) args);
	//check if any files were found
	if(args->head->next == NULL){
		printf("No Files Found!\n");
		return EXIT_SUCCESS;
	}
	//after return, sort data structure file in order of token count
	
	args->head->next = sortFiles(args->head->next);

	struct file* file1 = (struct file*) args->head->next;
	struct file* file2 = (struct file*) args->head->next->next;

	while(file1 != NULL) {
		while(file2 != NULL) {
			printf(RESET);
			double result = calculate(file1, file2);
			if(result >= 0 && result < 0.1) {
				printf(RED "%f ", result);
			} else if(result > 0.1 && result <= 0.15) {
				printf(YEL "%f ", result);
			} else if(result > 0.15 && result <= 0.2) {
				printf(GRN "%f ", result);
			} else if(result > 0.2 && result <= 0.25) {
				printf(CYN "%f ", result);
			} else if(result > 0.25 && result <= 0.3) {
				printf(BLU "%f ", result);
			} else {
				printf(WHT "%f ", result);
			}
			printf(RESET "for \"%s\" and \"%s\"\n", file1->path, file2->path);
			file2 = file2->next;
		}
		file1 = file1->next;

		if(file1->next != NULL)
			file2 = file1->next;
		else
			break;
	}

    printf(RESET);
    return EXIT_SUCCESS;
}
