#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "mymalloc.h"
/*simulated memory, begins with first 2 bytes of a block as metadata in order:  size of block (int16_t) and the size is negative if block not used, positive if used*/
static char vmem[4096] = {0,0};


void *mymalloc(size_t size, char* file, char* line){
	if(vmem[0] == 0 && vmem[1] == 0){//on first call, create initial metadata
		int16_t tmp = (int16_t) sizeof(vmem) -2;
		tmp*=-1;
		memcpy(&vmem[0],&tmp,2);
	}
	/*now we must loop through the blocks until we encounter metadata that is not used and large enough*/
	char* ptr = vmem;//pointer to begining of metadata for a block, starts at first memory location
	int16_t tmpsize = 0;
	while(ptr < &vmem[4093]){//ptr here can only point to metadata bytes
		int16_t cp = *(int16_t*) ptr;

		if(cp < 0){//an unused prev allocated block
			if(abs(cp) >= size){
				tmpsize = (int16_t) size;
				break;
			}
		}
		//printf("current increment malloc: %d\n",abs(cp));
		ptr+= abs(cp) + 2;

	}

	if(tmpsize==0){//out of memory!
		printf("Mymalloc: error in %s at line %s\n",file,line);
		return NULL;
	}else{
		int16_t cp = *(int16_t*) ptr;
		int16_t newblocksize = abs(cp) - tmpsize;
		char* tmp = ptr + tmpsize+2;
		memcpy(ptr,&tmpsize,2);
		ptr+=2;
		if((tmp+3) < &vmem[4095] && newblocksize>2){
			newblocksize-=2;
			newblocksize*=-1;
			memcpy(tmp,&newblocksize,2);
		}	
		
	}
	return ptr;
}




void merge(){//will defrag the memory space
	char* currentStart = vmem;
	char* currentEnd = vmem;
	while(currentStart<&vmem[4095]){

		if(*(int16_t*)currentStart < 0){//start of open block
			currentEnd=currentStart;
			while(*(int16_t*)currentEnd < 0){
				int16_t size = abs(*(int16_t*)currentEnd);
				currentEnd+=size+2;
			}
			int16_t diff = currentEnd - currentStart-2;
			diff*=-1;
			memcpy(currentStart,&diff,2);
		}
		currentStart+=abs(*(int16_t*)currentStart)+2;

	}
}
/*The free function will follow the metadata chain to see if given pointer matches any of the addresses*/
void myfree(void* ptr,char* file, char* line){
	
	char* current = vmem;
	ptr -= 2;
	while(current < &vmem[4095]){//while loop traverses through linked list
		int16_t cp = *(int16_t*) current;//get current block size
		if(current==ptr){ 

			if(cp<0){//address given to free was not allocated by mymalloc
				break;
			}
			cp*=-1;
			memcpy(ptr,&cp,2);
			merge();
			return;
		}
		//printf("abs of cp %d\n",abs(cp));
		
		current+= abs(cp) +2;

	}
	printf("Error freeing pointer in %s at line %s\n",file,line);
}

/*
void main(){

	char* tmp[200];
	for(int z = 0; z < 3; z++){
		for(int i=1;i<20;i++){
			tmp[i] = mymalloc(i,NULL,NULL);
			}
		char* ptr;
		ptr=tmp[1]-2;
		for(int i=1;i<20;i++){

			printf("malloc size %d\n",*((int16_t*) ptr));
			ptr+= *(int16_t*) ptr+2;
			}
		for(int i=1;i<20;i++){
			myfree(tmp[i],NULL,NULL);
			printf("free size %d\n",*((int16_t*)(tmp[i]-2)));
		}
	}

}
*/
