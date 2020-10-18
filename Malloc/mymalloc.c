#include "mymalloc.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
/*simulated memory, begins with first 2 bytes of a block as metadata in order:  size of block (int16_t) and the size is negative if block not used, positive if used*/
static char vmem[4096] = {0};


void *mymalloc(size_t size){
	if(vmem[0] == 0){//on first call, create initial metadata
		int16_t tmp = (int16_t) sizeof(vmem) -2;
		tmp*=-1;
		memcpy(&vmem[0],&tmp,2);
	}
	/*now we must loop through the blocks until we encounter metadata that is not used and large enough*/
	char *ptr = vmem;//pointer to begiing of metadata for a block, starts at first memory location
	while((size+2) > *(int16_t*) ptr){
		ptr+= *(int16_t*) ptr;
	}


}
