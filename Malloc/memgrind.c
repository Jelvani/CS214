#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <fcntl.h>
#include "mymalloc.h"

#define GRN "\x1B[32m"
#define RESET "\x1B[0m"

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

void printReport() {
	long longestA = 0, longestB = 0, longestC = 0, longestD = 0, longestE = 0;
	long totalA = 0, totalB = 0, totalC = 0, totalD = 0, totalE = 0;

	printf("\nThe following stress test goes from A-E running each test 50 times.\n\n");

	printf("TEST\t\tMEAN\t\tSLOWEST\t\tTOTAL\n");

	printf(GRN "A");
	for(int i = 0; i < 50; i++) {
		long temp = workloadA();
		if(temp > longestA)
			longestA = temp;
		totalA += temp;
	}
	printf(RESET "\t\t%ld\xC2\xB5s\t\t%ld\xC2\xB5s\t\t%ld\xC2\xB5s\n", (totalA / 50), longestA, totalA);

	printf(GRN "B");
	for(int i = 0; i < 50; i++) {
		long temp = workloadB();
		if(temp > longestB)
			longestB = temp;
		totalB += temp;
	}
	printf(RESET "\t\t%ld\xC2\xB5s\t\t%ld\xC2\xB5s\t\t%ld\xC2\xB5s\n", (totalB / 50), longestB, totalB);

	printf(GRN "C");
		for(int i = 0; i < 50; i++) {
		long temp = workloadC();
		if(temp > longestC)
			longestC = temp;
		totalC += temp;
	}
	printf(RESET "\t\t%ld\xC2\xB5s\t\t%ld\xC2\xB5s\t\t%ld\xC2\xB5s\n", (totalC / 50), longestC, totalC);

	printf(GRN "D");
		for(int i = 0; i < 50; i++) {
		long temp = workloadD();
		if(temp > longestD)
			longestD = temp;
		totalB += temp;
	}
	printf(RESET "\t\t%ld\xC2\xB5s\t\t%ld\xC2\xB5s\t\t%ld\xC2\xB5s\n", (totalD / 50), longestD, totalD);
	
	printf(GRN "E");
		for(int i = 0; i < 50; i++) {
		long temp = workloadE();
		if(temp > longestE)
			longestE = temp;
		totalE += temp;
	}
	printf(RESET "\t\t%ld\xC2\xB5s\t\t%ld\xC2\xB5s\t\t%ld\xC2\xB5s\n", (totalE / 50), longestE, totalE);
}

int main() {
	printReport();
	return 0;
}
