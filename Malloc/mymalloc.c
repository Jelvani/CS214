#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "mymalloc.h"

#define _MEMSIZE 4096
#define _BLOCKSIZE sizeof(int16_t)
#define _ERRORS 0 //1 for error printing, 0 for no error printing

/*simulated memory, begins with first 2 bytes of a block as metadata. Each memory block that is malloced, begins with 2 bytes as a int16_t with posative number representing size of block that is in use, and negative is size of block not in use.*/

static char vmem[_MEMSIZE] = {0,0};//magic number initialization

void *mymalloc(size_t size, char* file, int line){//returns null on error, will return pointer on succes, take in amount of bytes to be allocated
	if(size < 1) {
		return NULL;
	}
	if(vmem[0] == 0 && vmem[1] == 0){//on first call, create initial metadata
		int16_t tmp = (int16_t) sizeof(vmem) - _BLOCKSIZE;
		tmp*=-1;
		memcpy(&vmem[0],&tmp,_BLOCKSIZE);
	}
	/*now we must loop through the blocks until we encounter metadata that is not used and large enough*/
	char* ptr = vmem;//pointer to begining of metadata for a block, starts at first memory location
	int16_t tmpsize = 0;
	while(ptr < &vmem[_MEMSIZE-1]){//ptr here can only point to metadata bytes
		int16_t cp = *(int16_t*) ptr;

		if(cp < 0 && abs(cp) >= size){//if unused block that is large enough is found, stop.
			tmpsize = (int16_t) size;
			break;
		}
		ptr+= abs(cp) + _BLOCKSIZE;//increments ptr to next block
	}

	if(tmpsize == 0) {//ptr never found a block large enough, so we have an error
		if(_ERRORS){
			printf("Mymalloc: error in %s at line %d\n",file,line);
		}
		return NULL;
	} else {//here we actually allocated the ptr block and split it into 2 blocks if it has extra room
		int16_t cp = *(int16_t*) ptr;
		int16_t newblocksize = abs(cp) - tmpsize;
		char* tmp = ptr + tmpsize+_BLOCKSIZE;
		memcpy(ptr,&tmpsize,_BLOCKSIZE);//copy new block size to found block
		ptr+=_BLOCKSIZE;
		if((tmp+_BLOCKSIZE+1) < &vmem[_MEMSIZE-1] && newblocksize>_BLOCKSIZE){//go to end of extra space and create new metadata block that is free to use
			newblocksize-=_BLOCKSIZE;
			newblocksize*=-1;//negative for unused
			memcpy(tmp,&newblocksize,_BLOCKSIZE);
		}		
	}
	return ptr;
}

void merge(){//will defrag the memory space, as explained in 'readme.pdf'
	char* currentStart = vmem;
	char* currentEnd = vmem;
	while(currentStart < (&vmem[_MEMSIZE-1])){
		if(*(int16_t*)currentStart < 0){//start of open block
			currentEnd=currentStart;
			while(currentEnd < &vmem[_MEMSIZE-1]){//while finding all adjacent free blocks, save ending pointer
				if(*(int16_t*)currentEnd >= 0 ){
					break;
				}
				int16_t size = abs(*(int16_t*)currentEnd);
				currentEnd+=size+_BLOCKSIZE;
			}
			int16_t diff = currentEnd - currentStart - _BLOCKSIZE;//here we find the total size of the adjacent free blocks
			diff*=-1;
			memcpy(currentStart,&diff,_BLOCKSIZE);	
		}
		currentStart+=abs(*(int16_t*)currentStart)+_BLOCKSIZE;//find next adjacent free blocks
	}
}

/*The free function will follow the metadata chain to see if given pointer matches any of the addresses*/
void myfree(void* ptr,char* file, int line){//takes in a pointer to be free, returns nothing.
	if(ptr == NULL){
		return;
	}

	char* current = vmem;
	ptr -= _BLOCKSIZE;
	while(current < &vmem[_MEMSIZE-1]){//while loop traverses through linked list
		int16_t cp = *(int16_t*) current;//get current block size
		if(current==ptr) { 
			if(cp<0){//address given to free was not allocated by mymalloc
				break;
			}
			cp*=-1;//here we free but just switching metadata to a negative
			memcpy(ptr,&cp,_BLOCKSIZE);
			merge();//look for consecutve free blocks and merge
			return;
		}
		
		current+= abs(cp) +_BLOCKSIZE;//go to next block
	}
	if(_ERRORS){
		printf("Myfree: error in %s at line %d\n",file,line);
	}
}

void printPointers(){//function prints all current pointers in vmem, used for debugging
	char* currentStart = vmem;
	while(currentStart<&vmem[_MEMSIZE-1]){
		printf("Block exists at: %p\n",currentStart);
		currentStart+=abs(*(int16_t*)currentStart)+_BLOCKSIZE;
	}
}
