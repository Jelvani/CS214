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

//according to menny, we cannot have our mutex as a global, so we pass this struct to each thread
struct thread{
	char* dir;
	pthread_mutex_t* lock;
};

void* findDir(void* directory) {
	struct thread* args = (struct thread*) directory;
    DIR *dir;
    struct dirent *dp;
	pthread_t* thread_list = (pthread_t*) malloc(sizeof(pthread_t));
	int trdCnt = 1;
	printf("%s\n",args->dir);
    //checks to see if can open directory, if not fail it.
    if((dir = opendir(args->dir)) == NULL) {
		printf("ERR: cannot open current directory");
		exit(1);
	}
	printf(RESET);
    while((dp = readdir(dir)) != NULL) {
        

        if(dp->d_type == DT_DIR) {

			//check for previous and current dir links
            if(strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) {
                continue;
            }
                printf(BLU "Found dir: %s\n", dp->d_name);
                char* temp = (char*) malloc(strlen(args->dir) + strlen(dp->d_name) + 2);
				strcpy(temp,args->dir);
                strcat(temp, "/");
                strcat(temp, dp->d_name);
				struct thread new_args;
				new_args.dir = temp;
				
				pthread_create(thread_list+trdCnt-1,NULL,findDir,(void*) &new_args);
				trdCnt++;
				thread_list = (pthread_t*) realloc(thread_list,sizeof(pthread_t)*trdCnt);
		
        } else if(dp->d_type == DT_REG) {//found a regular file
			//check for exectuable later, not specified in instructions
			printf(GRN "Found file: %s\n", dp->d_name);

        }
        printf(RESET);
    }
    closedir(dir);

    
	free(args->dir);
	free(thread_list);
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
	
	struct thread args;
	args.lock = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
        //initializes mutex lock for synch
        if(pthread_mutex_init(args.lock, NULL) != 0) { 
            printf("mutex init has failed\n"); 
            return 1; 
        } 

    char* fullpath;
	
	if((fullpath=realpath(argv[1], NULL))==NULL){
		printf(RED "The path: '%s' is invalid!\n",argv[1]);
		printf(RESET);
		return EXIT_FAILURE;
	}
	args.dir = fullpath;
		//does not need to be thread as per instruction (1.c)
	
    findDir((void*) &args);
    printf(RESET);
    return EXIT_SUCCESS;
}
