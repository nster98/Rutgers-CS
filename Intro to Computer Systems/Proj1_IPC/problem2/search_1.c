// Nathan Glikman
// Gregory Giovannini
// Problem 2 Question 1
// search_1.c

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>

int search(int* arr, int size, int factor, int key, int* foundIndexes);


#define KEY -50
#define ULIMIT 31830

// Single process case.
int search(int* arr, int size, int factor, int key, int* foundIndexes)
{
	int foundCount = 0;
	int max = arr[0];
	size_t i;

	printf("I am process %d, my parent is %d\n", getpid(), getppid());

	for (i = 0; i < size; i++)
	{
		// Max check.
		if (arr[i] > max)
		{
			max = arr[i];
		}
		// Key check.
		if (arr[i] == key)
		{
			foundIndexes[foundCount++] = i;
			printf("I am process %d and I found the key at %d\n", getpid(), foundIndexes[foundCount - 1]);
		}
		// All found check.
		if (foundCount == 3)
		{
			break;
		}
	}

	printf("Max = %d\n", max);

	// Return -1 if not all 3 keys were found.
	int retVal = (foundCount == 3) ? 0 : -1;
	return retVal;
}


int main(int argc, char* argv[])
{
	// Error check on input, the length of the list.
    if (argc != 2)
    {
        printf("Invalid arguments: 1 expected, %d provided. \nRequires a single number, the length of the number list to search.\n", argc - 1);
        return -1;
    }
    int LIST_SIZE = atoi(argv[1]);

	int arr[LIST_SIZE];

	// Read in the input list from the file.
	FILE* file = fopen("arrayNums.txt", "r");
	int i;
	for (i = 0; i < LIST_SIZE; i++)
	{
		if (fscanf(file, "%d", &arr[i]) != 1)
			break;
	}
	fclose(file);

	// Set the keys at each point in the array.
	int key = KEY;
	arr[LIST_SIZE / 4] = key;
	arr[LIST_SIZE / 2] = key;
	arr[(3 * LIST_SIZE) / 4] = key;

	// TODO: calculate the factor here.
	int factor = 4; // Amount of processes to make
	int ulimit = ULIMIT; // ulimit set by DSV 

	if (factor > ulimit)
	{
		printf("Went over ulimit, stopping program...\n");
		return -1;
	}


	// Start timing here.
	struct timeval start, end, diff;
    gettimeofday(&start, NULL);

	int* foundIndexes = (int*) malloc(sizeof(int) * 3);
	search(arr, LIST_SIZE, factor, key, foundIndexes);

	for (i = 0; i < 3; i++)
	{
		printf("Key found at index: %d\n", foundIndexes[i]); // DOESNT WORK
	}

	// Stop timing here.
	gettimeofday(&end, NULL);
    timersub(&end,   &start, &diff);

	printf("Search took %'8.3f ms.\n", diff.tv_sec*1000.0 + diff.tv_usec/1000.0);

	free(foundIndexes);

	// THOUGHTS
	//
	// There needs to be something like an array for the process to put its found index for the key into
	// They should also have a maximum that is sent to each process whenever one of them finds something larger
}
