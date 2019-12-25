// Nathan Glikman		Andrew Cater
// memgrind.c

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include "mymalloc.h"

float testA()
{
	// A: malloc() 1 byte and immediately free it - do this 150 times
	struct timeval start, end;

	gettimeofday(&start, NULL);
	
	for (int i = 0; i < 150; i++)
	{
		char* x = (char*) malloc(1);
		free(x);
	}

	gettimeofday(&end, NULL);

	return (float)((end.tv_sec - start.tv_sec) * 10000000 + (end.tv_usec - start.tv_usec) * 0.000001);
}

float testB()
{
	// B: malloc() 1 byte, store the pointer in an array - do this 150 times
	// Once you've malloc()ed 50 byte chunks, then free() the 50 1 byte pointers one by one
	
	struct timeval start, end;

	gettimeofday(&start, NULL);
	
	char* arr0[150];
	int j = 0;
	for (int i = 0; i < 3; i++)
	{
			j =0;
			while(j < 50)
			{//44
				char* y = (char*) malloc(1);
				arr0[j] = y;
				j++;
			}
			j--;
			for(int k = 0; k < 50; k++)
			{//51
				free(arr0[j]);
				j--;
			}
	}

	gettimeofday(&end, NULL);

	return (float)((end.tv_sec - start.tv_sec) * 10000000 + (end.tv_usec - start.tv_usec) * 0.000001);

}

float testC()
{
	/* C: Randomly choose between a 1 byte malloc() or free()ing a 1 byte pointer
	 * 		> do this until you have allocated 50 times
	 * - Keep track of each operation so that you eventually malloc() 50 bytes, in total
	 *   	> if you have already allocated 50 times, disregard the random and just free() on each
	 *    		iteration
	 * - Keep track of each operation so that you eventually free() all pointers
	 *    	> don't allow a free() if you have no pointers to free() 
	 */
	struct timeval start, end;

	gettimeofday(&start, NULL);
		
	
	unsigned long int rand = time(NULL);


	int count = 0; int freeCount = 0;
	char* a;
	char* arr1[50];
	int arrCount = 0;
	while (count < 50)
	{
		rand = (unsigned int)((rand * 1637832 + 268945) / 12456 % 23123);	
		rand = rand % 2;
			
		if (rand == 1)
		{
			char* a = (char*) malloc(1);
			arr1[arrCount] = a;
			count++;
			freeCount++;
			arrCount++;
		}
		else if (a != NULL)
		{
			free(a);
			freeCount--;
		}
	}
	for (int i = 0; i < 50; i++)
	{
		if (arr1[i] != NULL)
		{
			free(arr1[i]);
		}
	}

	gettimeofday(&end, NULL);

	return (float)((end.tv_sec - start.tv_sec) * 10000000 + (end.tv_usec - start.tv_usec) * 0.000001);

}

float testD()
{
	/*  D: Randomly choose between a randomly-sized malloc() or free()ing a pointer – do this many
	 *  times (see below)
	 *  - Keep track of each malloc so that all mallocs do not exceed your total memory capacity
	 *  - Keep track of each operation so that you eventually malloc() 50 times
	 *  - Keep track of each operation so that you eventually free() all pointers
	 *  - Choose a random allocation size between 1 and 64 bytes
	 */
	
	struct timeval start, end;
	gettimeofday(&start, NULL);

	int timesMalloced = 0; 						// Amount of times malloced, should go to 50
	int randNum = 0;							// The random number 
	int memoryAllocation = 0;					// The random allocation size, from 1-64
	int amountMemoryLeft = BLOCK_SIZE;			// Keeps track of how much memory is left
	char* memArr[200];							// Array to hold the pointers
	int sizeArr[200];							// Array to hold size in tangent to the memory array
	int arrCounter = 0;							// Array counter
	unsigned long int randGen = time(NULL);		// Random number generator

	while (timesMalloced < 50)
	{
		randGen = (unsigned int)(((randGen * 123948345 + 98765) / 12345) % 40596);
		randNum = randGen % 2;

		if (randNum == 1) 	// Malloc time
		{
			memoryAllocation = (randGen % 64) + 1;

			if ((memoryAllocation + sizeof(struct node)) <= amountMemoryLeft)
			{
				amountMemoryLeft = amountMemoryLeft - (memoryAllocation + sizeof(struct node));
				memArr[arrCounter] = malloc(memoryAllocation);
				sizeArr[arrCounter] = memoryAllocation + sizeof(struct node);

				timesMalloced++;
				arrCounter++;

			}
		}
		else				// Free time
		{
			if (arrCounter > 0) 
			{
				amountMemoryLeft = amountMemoryLeft + sizeArr[arrCounter - 1];
				free(memArr[arrCounter - 1]);

				arrCounter--;
			}
		} 
	}
	while (arrCounter > 0) 
	{
		amountMemoryLeft = amountMemoryLeft + sizeArr[arrCounter - 1];
		free(memArr[arrCounter - 1]);

		arrCounter--;
	}

	gettimeofday(&end, NULL);
	
	return (float)((end.tv_sec - start.tv_sec) * 10000000 + (end.tv_usec - start.tv_usec) * 0.000001);
}

//Mallocs 8 size 512 spaces. This will fail as it will be short on the last run.
float testE()
{
	struct timeval start, end;
	gettimeofday(&start, NULL);
	char* arr0[10];
	int j = 0;
	for(int i = 0; i < 8; i++)
	{
		char* a = (char*) malloc(512);
		if(a != NULL)
		{
			arr0[j] = a;		
			j++;
		}	
	}
	j--;
	while(j >= 0)
	{
		free(arr0[j]);
		j--;
	}
	gettimeofday(&end, NULL);
	return (float)((end.tv_sec - start.tv_sec) * 10000000 + (end.tv_usec - start.tv_usec) * 0.000001);


}

float testF()
{
	struct timeval start, end;
	gettimeofday(&start, NULL);

	// Welcome to Wendy's
	char* burger = (char*) malloc(1024 - sizeof(struct node));
	char* frenchfry = (char*) malloc(1024 - sizeof(struct node));
	char* nuggets = (char*) malloc(1024 - sizeof(struct node));
	char* frosty = (char*) malloc(1024 - sizeof(struct node) - 8);

	free(frosty);
	free(nuggets);
	free(frenchfry);
	free(burger);

	char* spicychicken = (char*)malloc(2048 - sizeof(struct node));
	char* water = (char*)malloc(2048 - sizeof(struct node) - 8);

	free(spicychicken);
	free(water);

	char* bigmac = (char*)malloc(4096 - sizeof(struct node) - 8);

	free(bigmac);

	// Have a nice day!

	gettimeofday(&end, NULL);
	return (float)((end.tv_sec - start.tv_sec) * 10000000 + (end.tv_usec - start.tv_usec) * 0.000001);

}


int main(int argc, char* argv[])
{
	int timesToRun = 100;

	printf("TEST A\n");
	float sumA = 0.0;
	for (int i = 0; i < timesToRun; i++)
	{
		float a = testA();
		if (a < 10)
			sumA = sumA + a;
	}
	float timeA = sumA / timesToRun;

	printf("TEST B\n");
	float sumB = 0.0;
	for (int i = 0; i < timesToRun; i++)
	{
		float b = testB();
		if (b < 10)
			sumB = sumB + b;
	}
	float timeB = sumB / timesToRun;

	printf("TEST C\n");
	float sumC = 0.0;
	for (int i = 0; i < timesToRun; i++)
	{
		float c = testC();
		if (c < 10)
			sumC = sumC + c;
	}
	float timeC = sumC / timesToRun;

	printf("TEST D\n");
	float sumD = 0.0;
	for (int i = 0; i < timesToRun; i++)
	{
		float d = testD();
		if (d < 10)
			sumD = sumD + d;
	}
	float timeD = sumD / timesToRun;

	printf("TEST E\n");
	float sumE = 0.0;
	for (int i = 0; i < timesToRun; i++)
	{
		float e = testE();
		if (e < 10)
			sumE = sumE + e;
	}
	float timeE = sumE / timesToRun;

	printf("TEST F\n");
	float sumF = 0.0;
	for (int i = 0; i < timesToRun; i++)
	{
		float f = testF();
		if (f < 10)
			sumF = sumF + f;
	}
	float timeF = sumF / timesToRun;

	printf("Time to run Test A: %.8f microseconds\n", timeA);
	printf("Time to run Test B: %.8f microseconds\n", timeB);
	printf("Time to run Test C: %.8f microseconds\n", timeC);
	printf("Time to run Test D: %.8f microseconds\n", timeD);
	printf("Time to run Test E: %.8f microseconds\n", timeE);
	printf("Time to run Test F: %.8f microseconds\n", timeF);

	return 0;
}


