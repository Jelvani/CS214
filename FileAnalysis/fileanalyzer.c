#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>

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


void* fileHandle(void* directory){
	struct threadArg* args = (struct threadArg*) directory;
	printf("FILE HANDLE: %s\n",args->dir);
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
	struct file files;
	files.next=NULL;
	files.token=NULL;
	
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
	args->head = &files;
		//does not need to be thread as per instruction (1.c)
	
    findDir((void*) args);
    printf(RESET);
    return EXIT_SUCCESS;
}
