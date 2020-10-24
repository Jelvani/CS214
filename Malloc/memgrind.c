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
 * 
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
 * 
 * once number of allocations has equaled to 120
 * 		free remainder of array
 * 
 * record t_end using gettimeofday
 * This will return value in microseconds.
 */
long workloadC() {
	struct timeval t_start, t_end;
	time_t t;
	char* testArray[120];
	int counter = 0;
	int numberAllocated = 0;
	srand((unsigned) time(&t));
	gettimeofday(&t_start, NULL);
	while(counter != 240) {
		int random = (rand() % 2);

		if(numberAllocated == 120)
			break;

		if(random == 0) {
			testArray[numberAllocated] = malloc(1);
			numberAllocated++;
		} else {
			if(numberAllocated > 0) {
				free(testArray[numberAllocated - 1]);
				numberAllocated--;
			}
		}
		counter++;
	}

	for(int i = 0; i < numberAllocated; i++) {
		free(testArray[i]);
	}

	gettimeofday(&t_end, NULL);
	return getTime(t_start, t_end);
}

/**
 * workloadD()
 * record t_start using gettimeofday()
 * 
 * We will be testing various failures to ensure mymalloc/myfree are able to handle cases
 * that are not meant to work.
 * 
 * record t_end using gettimeofday()
 * This will return value in microseconds.
 */ 
long workloadD() {
	struct timeval t_start, t_end;
	int counter = 0;
	gettimeofday(&t_start, NULL);

	//p allocation to big -> return nothing 							THIS WORKS [FAILSAFE ADDED]
	char* p = (char*) malloc(5000);

	//x free'd first time, cannot free already free'd pointer			NOT HANDLED YET
	char* x = (char*) malloc(100);
	free(x);
	free(x);

	//t trying to free memory not allocated to t.						NOT HANDLED YET
	char* t = (char*) malloc(200);
	free(t + 10);

	//j never allocated by malloc, thus cannot free.					NOT HANDLED YET
	int *j;
	free(j);

	gettimeofday(&t_end, NULL);
	return getTime(t_start, t_end);
}

long workloadE() {
	struct timeval t_start, t_end;
	int counter = 0;
	gettimeofday(&t_start, NULL);

	gettimeofday(&t_end, NULL);
	return getTime(t_start, t_end);
}

void printReport() {
	long longestA = 0, longestB = 0, longestC = 0, longestD = 0, longestE = 0;
	long totalA = 0, totalB = 0, totalC = 0, totalD = 0, totalE = 0;

	for(int i = 0; i < 50; i++) {
		long tempA = workloadA();
		if(tempA > longestA)
			longestA = tempA;
		totalA += tempA;

		long tempB = workloadB();
		if(tempB > longestB)
			longestB = tempB;
		totalB += tempB;

		long tempC = workloadC();
		if(tempC > longestC)
			longestC = tempC;
		totalC += tempC;

		long tempD = workloadD();
		if(tempD > longestD)
			longestD = tempD;
		totalD += tempD;

		long tempE = workloadE();
		if(tempE > longestE)
			longestE = tempE;
		totalE += tempE;
	}

	printf("\nThe following stress test goes from A-E running each test 50 times.\n\n");
	printf("TEST\t\tMEAN\t\tSLOWEST\t\tTOTAL\n");

	printf(GRN "A");
	printf(RESET "\t\t%ld\xC2\xB5s\t\t%ld\xC2\xB5s\t\t%ld\xC2\xB5s\n", (totalA / 50), longestA, totalA);

	printf(GRN "B");
	printf(RESET "\t\t%ld\xC2\xB5s\t\t%ld\xC2\xB5s\t\t%ld\xC2\xB5s\n", (totalB / 50), longestB, totalB);

	printf(GRN "C");
	printf(RESET "\t\t%ld\xC2\xB5s\t\t%ld\xC2\xB5s\t\t%ld\xC2\xB5s\n", (totalC / 50), longestC, totalC);

	printf(GRN "D");
	printf(RESET "\t\t%ld\xC2\xB5s\t\t%ld\xC2\xB5s\t\t%ld\xC2\xB5s\n", (totalD / 50), longestD, totalD);
	
	printf(GRN "E");
	printf(RESET "\t\t%ld\xC2\xB5s\t\t%ld\xC2\xB5s\t\t%ld\xC2\xB5s\n", (totalE / 50), longestE, totalE);
}

int main() {
	printReport();
	return 0;
}
