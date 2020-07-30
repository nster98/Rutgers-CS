// Gregory Giovannini
// Problem 1 Question 1
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
#include <string.h>
#include <pthread.h>

int search(int *arr, int size, int xThreads, int key, int *foundIndexes);
void *atomicSearch(void *threadArgs);

#define KEY -50

typedef struct {
	int foundMax;
	int foundCount;
	int *foundIndexes;
} ThreadOutputs;

typedef struct {
	ThreadOutputs *threadOutputs;
	int *arr;
	int size;
	int startIndex;
	int stopIndex;
	int key;
} ThreadInputs;


int search(int *arr, int size, int xThreads, int key, int *foundIndexes)
{
	pthread_t *threadHandles = calloc(xThreads, sizeof (pthread_t));  // Array of thread handles for each thread.
    ThreadInputs *threadInputs = calloc(xThreads, sizeof (ThreadInputs));  // Array of thread arguments for each thread.
	ThreadOutputs *threadOutputs = calloc(xThreads, sizeof (ThreadOutputs));  // Array of thread outputs for each thread.
	int splitSize = size / xThreads;  // Number of elements for each thread to search, not counting leftover.
	int foundCount = 0;
	int max = arr[0];
	size_t i, j;
	int err;

	// Check for error with calloc.
	if (threadHandles == NULL || threadInputs == NULL || threadOutputs == NULL)
	{
		printf("Error allocating memory. Exiting...\n");
		exit(-1);
	}

	// Fill the Thread args.
	for (i = 0; i < xThreads; i++)
	{
		threadInputs[i].threadOutputs = &(threadOutputs[i]);
		threadInputs[i].arr = arr;
		threadInputs[i].size = size;
		threadInputs[i].startIndex = splitSize * i;
		threadInputs[i].stopIndex = (i < xThreads - 1) ?
									threadInputs[i].startIndex + splitSize :
									threadInputs[i].startIndex + splitSize + (size % xThreads);  // Give the last thread the remainder of the elements if division of labor was uneven.
		threadInputs[i].key = key;

		// Allocate output space.
		threadInputs[i].threadOutputs -> foundIndexes = calloc(3, sizeof(int));
		// Check for error with calloc.
		if (threadInputs[i].threadOutputs -> foundIndexes == NULL)
		{
			printf("Error allocating memory. Exiting...\n");
			exit(-1);
		}
	}

	// Spin up threads.
	for (i = 0; i < xThreads; i++)
	{
		err = pthread_create(&threadHandles[i], NULL, (void * (*)(void *)) atomicSearch, (void *) &threadInputs[i]);
		if (err)
		{
			printf("Error creating thread #%zu.\n", i);
			//exit(-1);
		}
	}

	// Join threads in order and collect the results.
	for (i = 0; i < xThreads; i++)
	{
		ThreadOutputs *currentOutput;
		err = pthread_join(threadHandles[i], (void **) &currentOutput);
		if (err)
		{
			printf("Error joining thread #%zu.\n", i);
			//exit(-1);
		}
		else 
		{
			for (j = 0; j < currentOutput -> foundCount; j++)
			{
				foundIndexes[foundCount++] = currentOutput -> foundIndexes[j];
			}

			if (currentOutput -> foundMax > max)
			{
				max = currentOutput -> foundMax;
			}
		}
	}

	printf("Max = %d\n", max);

	// Free the End!
	free(threadHandles);
    free(threadInputs);

	for (i = 0; i < xThreads; i++)
	{
		free(threadOutputs[i].foundIndexes);
	}
	free(threadOutputs);

	//printf("No errors so far.\n");

	// Return -1 if not all 3 keys were found.
	int retval = (foundCount == 3) ? 0 : -1;
	return retval;
}


void* atomicSearch(void *threadArgs)
{
	// Unpack the parameters.
	ThreadInputs *threadInputs = (ThreadInputs *) threadArgs;
	ThreadOutputs *threadOutputs = threadInputs -> threadOutputs;
	int* arr = threadInputs -> arr;
	int size = threadInputs -> size;
	int startIndex = threadInputs -> startIndex;
	int stopIndex = threadInputs -> stopIndex;
	int key = threadInputs -> key;

	printf("I am pthread %d, searching from %d to %d.\n", (int) pthread_self(), startIndex, stopIndex);

	// Initialize the search.
	int foundCount = 0;
	int max = arr[startIndex];
	size_t i;

	for (i = startIndex; i < size && i < stopIndex; i++)
	{
		// Max check.
		if (arr[i] > max)
		{
			max = arr[i];
		}
		// Key check.
		if (arr[i] == key)
		{
			threadOutputs -> foundIndexes[foundCount++] = i;
			threadOutputs -> foundCount = foundCount;
			printf("I am pthread %d and I found the key at %zu\n", (int) pthread_self(), i);
		}
	}

	// Save the maximum.
	threadOutputs -> foundMax = max;

	pthread_exit((void *) threadOutputs);

	return NULL;  // To satisfy compiler, because GCC is a stinky poohead.
}


int main(int argc, char* argv[])
{
	// Error check on input, the length of the list.
    if (argc != 3)
    {
        printf("Invalid arguments: 2 expected, %d provided. \nRequires two integers: the length of the list to search, and the number of threads that can be created.\n", argc - 1);
        return -1;
    }
    int listSize = atoi(argv[1]);
	int xThreads = atoi(argv[2]);

	// Check to make sure the number of threads requested is not too high.
	// TODO

	int arr[listSize];

	// Read in the input list from the file.
	FILE* file = fopen("arrayNums.txt", "r");
	int i;
	for (i = 0; i < listSize; i++)
	{
		if (fscanf(file, "%d", &arr[i]) != 1)
			break;
	}

	// Set the keys at each point in the array.
	int key = KEY;
	arr[listSize / 4] = key;
	arr[listSize / 2] = key;
	arr[(3 * listSize) / 4] = key;


	// Start timing here.
	struct timeval start, end, diff;
    gettimeofday(&start, NULL);

	int* foundIndexes = (int*) malloc(sizeof(int) * 3);

	// Check for error with malloc.
	if (foundIndexes == NULL) exit(-1);

	int result = search(arr, listSize, xThreads, key, foundIndexes);
	if (result == -1)
	{
		printf("Not all 3 keys found.\n");
	}

	for (i = 0; i < 3; i++)
	{
		printf("Key found at index: %d\n", foundIndexes[i]);
	}

	// Stop timing here.
	gettimeofday(&end, NULL);
    timersub(&end,   &start, &diff);

	printf("Search took %'8.3f ms.\n", diff.tv_sec*1000.0 + diff.tv_usec/1000.0);

	free(foundIndexes);
}
