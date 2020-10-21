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
	char *ptr = vmem;//pointer to begiing of metadata for a block, starts at first memory location
	int16_t tmpsize = 0;
	while(ptr < &vmem[4093]){//ptr here can only point to metadata bytes
		if(*(int16_t*) ptr <0){//an unused prev allocated block
			if(abs(*(int16_t*) ptr) >= (size)){
				tmpsize = (int16_t) size;
				break;
			}
		}
		ptr+= abs(*(int16_t*) ptr)+2;

	}

	if(tmpsize==0){//out of memory!
		return NULL;
	}else{
		int newblock = (abs(*(int16_t*) ptr) - tmpsize)*-1;
		char* tmp = ptr + tmpsize+2;
		memcpy(ptr,&tmpsize,2);
		ptr+=2;
		if((tmp+3) < &vmem[4095]){
			memcpy(tmp,&newblock,2);
		}	
		
	}
	return ptr;
}


/*The free function will follow the metadata chain to see if given pointer matches any of the addresses*/
void myfree(void* ptr,char* file, char* line){

	void* current = vmem;
	ptr-=2;
	while(current < &vmem[4093]){
		if(current ==  ptr){
			int16_t tmp = *(int16_t*) ptr;
			if(tmp<0){//address was not allocated by mymalloc
				break;
			}
			tmp*=-1;
			memcpy(ptr,&tmp,2);
			return;
		}
		current+= abs(*(int16_t*)current) +2;
	}
	printf("Error freeing pointer in %s at line %s\n",file,line);
}

/*
void main(){


	for(int i=0;i<2000;i++){
		
		char* tmp = mymalloc(1,NULL,NULL);
		myfree(tmp,NULL,NULL);
	}


}
*/
