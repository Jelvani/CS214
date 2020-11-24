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
}

void* findDir(void* args) {
    DIR *dir;
    struct dirent *dp;
    struct stat sb;
    int vCounter = 0;
    char* values[100];
	printf("%s\n",directory);
    //checks to see if can open directory, if not fail it.
    if((dir = opendir(directory)) == NULL) {
		printf("ERR: cannot open current directory");
		exit(1);
	}

    while((dp = readdir(dir)) != NULL) {
        printf(RESET);

        if(dp->d_type == DT_DIR) {
			//check for previous and current dir links
            if(strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) {
                continue;
            } else {
                printf(BLU "Found dir: %s\n", dp->d_name);
                char* temp = directory;
                strcat(temp, "/");
                strcat(temp, dp->d_name);
                values[vCounter++] = temp;
            }
		
        } else {
            //skip executables since we cannot read them
            if(stat(dp->d_name, &sb) == 0 && sb.st_mode & S_IXUSR) {
				continue;
			} else { 
				printf(GRN "Found file: %s\n", dp->d_name);
			}
        }
        printf(RESET);
    }
    closedir(dir);

    for(int i = 0; i < vCounter; i++) {
        findDir(values[i]);
    }
}

int main(int argc, char *argv[]) {
    //Checks for args count
    if(argc != 2) {
        printf(RESET);
        printf("ERR: INVALID ARG COUNT\nTo properly run the program, enter a directory to start scanning.\n");
        return EXIT_FAILURE;
    }

    DIR *dir;
	struct file files;
	files.next=NULL;
	files.token=NULL;
	struct threads args;
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
    findDir(&args);
    free(fullpath);//realpath uses malloc, so we must deallocate used memory
    printf(RESET);
    return EXIT_SUCCESS;
}
