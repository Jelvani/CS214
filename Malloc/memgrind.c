#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "mymalloc.h"

/**
 * workloadA()
 * 	record t_start using timeofday()
 *	for 120 cycles,
 *		malloc() 1 byte, immediately free()
 *	record t_end using gettimeofday()
 *	return t_end - t_start
 *	This will return value in microseconds.
 */
long workloadA() {
	struct timeval t_start, t_end;
	int counter = 0;
	gettimeofday(&t_start, NULL);
	while(counter < 120) {
		char* temp = malloc(1);
		free(temp);
		counter++;
	}
	gettimeofday(&t_end, NULL);
	return (t_end.tv_usec - t_start.tv_usec);
}

/**
 * workloadB()
 * 	record t_start using timeofday()
 *	for 120 cycles,
 *		fill up testArray by mymalloc() 1 byte
		free entire array with myfree() 1 by 1
 *	record t_end using gettimeofday()
 *	return t_end - t_start
 *	This will return value in microseconds.
 */
long workloadB() {
	struct timeval t_start, t_end;
	char* testArray[120];
	int counter = 0;
	gettimeofday(&t_start, NULL);
	while(counter < 120) {
		for(int i = 0; i < 120; i++) {
			testArray[i] = malloc(1);
		}

		for(int j = 0; j < 120; j++) {
			free(testArray[j]);
		}

		counter++;
	}
	gettimeofday(&t_end, NULL);
	return (t_end.tv_usec - t_start.tv_usec);
}

long workloadC() {
	struct timeval t_start, t_end;
	int counter = 0;
	gettimeofday(&t_start, NULL);
	while(counter < 240) {
		counter++;
	}
	gettimeofday(&t_end, NULL);
	return (t_end.tv_usec - t_start.tv_usec);
}

long workloadD() {
	struct timeval t_start, t_end;
	int counter = 0;
	gettimeofday(&t_start, NULL);
	while(counter < 1) {
		counter++;
	}
	gettimeofday(&t_end, NULL);
	return (t_end.tv_usec - t_start.tv_usec);
}

long workloadE() {
	struct timeval t_start, t_end;
	int counter = 0;
	gettimeofday(&t_start, NULL);
	while(counter < 1) {
		counter++;
	}
	gettimeofday(&t_end, NULL);
	return (t_end.tv_usec - t_start.tv_usec);
}

int main() {
	printf("Workload A average time: %ld\xC2\xB5s\n", workloadA());
	printf("Workload B average time: %ld\xC2\xB5s\n", workloadB());
	printf("Workload C average time: %ld\xC2\xB5s\n", workloadC());
	printf("Workload D average time: %ld\xC2\xB5s\n", workloadD());
	printf("Workload E average time: %ld\xC2\xB5s\n", workloadE());
	return 0;
}
