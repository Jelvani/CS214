#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <fcntl.h>
#include "mymalloc.h"

#define GRN "\x1B[32m"
#define RESET "\x1B[0m"

/**
 * getTime(timeval t_start, timeval t_end)
 * calculate proper calculation of microseconds due to overflow buffer of tv_usec
 */
long getTime(struct timeval t_start, struct timeval t_end) {
	long secs = t_end.tv_sec - t_start.tv_sec;
	long micsec = t_end.tv_usec - t_start.tv_usec;

	if(micsec < 0)
		secs -= 1;

	return (secs * 1000000) + abs(micsec);
}

/**
 * workloadA()
 * 	record t_start using gettimeofday()
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
	return getTime(t_start, t_end);
}

/**
 * workloadB()
 * 	record t_start using gettimeofday()
 *	for 120 cycles,
 *		fill up testArray by mymalloc() 1 byte
		free entire array with myfree() 1 by 1
 *	record t_end using gettimeofday()
 *	This will return value in microseconds.
 */
long workloadB() {
	struct timeval t_start, t_end;
	char* testArray[120];
	gettimeofday(&t_start, NULL);
	for(int i = 0; i < 120; i++) {
		testArray[i] = malloc(1);
	}

	for(int j = 0; j < 120; j++) {
		free(testArray[j]);
	}
	gettimeofday(&t_end, NULL);
	return getTime(t_start, t_end);
}

/**
 * workloadC()
 * record t_start using gettimeofday()
 * until number of allocations (mymalloc) equals 120
 * 		randomly generate a number -> 0 or 1
 * 		if 0:
 * 			malloc(1) and store into array
 * 			increase number of allocations
 * 		if 1:
 * 			if number of allocations is greater than 0 (something has been malloc'd prior)
 * 				free last malloc'd position from array()
 * 			else
 * 				do nothing
 * record t_end using gettimeofday
 * This will return value in microseconds.
 */
long workloadC() {
	struct timeval t_start, t_end;
	time_t t;
	char* testArray[120];
	int numberAllocated = 0;
	int counter = 0;
	srand((unsigned) time(&t));
	gettimeofday(&t_start, NULL);
	while(numberAllocated != 120) {
		int random = (rand() % 2);
		if(random == 0) {
			testArray[numberAllocated] = malloc(1);
			numberAllocated++;
		} else {
			if(numberAllocated > 0) {
				free(testArray[numberAllocated - 1]);
				numberAllocated--;
			}
		}
	}

	for(int i = 0; i < numberAllocated; i++) {
		free(testArray[i]);
	}

	gettimeofday(&t_end, NULL);
	return getTime(t_start, t_end);
}

long workloadD() {
	struct timeval t_start, t_end;
	int counter = 0;
	gettimeofday(&t_start, NULL);
	while(counter < 1) {
		counter++;
	}
	gettimeofday(&t_end, NULL);
	return getTime(t_start, t_end);
}

long workloadE() {
	struct timeval t_start, t_end;
	int counter = 0;
	gettimeofday(&t_start, NULL);
	while(counter < 1) {
		counter++;
	}
	gettimeofday(&t_end, NULL);
	return getTime(t_start, t_end);
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
