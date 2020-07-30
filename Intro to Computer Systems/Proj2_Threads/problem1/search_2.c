// Gregory Giovannini
// Problem 1 Question 2
// search_2.c

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

int search(int *arr, int size, int elementsToSearch, int key, int *foundIndexes);
void *recursiveSearch(void *threadArgs);
void *atomicSearch(void *threadArgs);

#define KEY -50
#define MIN_LIST_SIZE 1000

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
	int elementsToSearch;
} ThreadInputs;


int search(int *arr, int size, int elementsToSearch, int key, int *foundIndexes)
{
	// This is the search initializer, should only be called once.
	pthread_t *threadHandles = calloc(1, sizeof (pthread_t));  // Array of thread handles for each thread.
    ThreadInputs *threadInputs = calloc(1, sizeof (ThreadInputs));  // Array of thread arguments for each thread.
	ThreadOutputs *threadOutputs = calloc(1, sizeof (ThreadOutputs));  // Array of thread outputs for each thread.
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

	// Fill the Main Thread args.
	threadInputs[0].threadOutputs = &(threadOutputs[0]);
	threadInputs[0].arr = arr;
	threadInputs[0].size = size;
	threadInputs[0].startIndex = 0;
	threadInputs[0].stopIndex = size - 1;
	threadInputs[0].key = key;
	threadInputs[0].elementsToSearch = size;

	// Set the output space to be the foundIndexes pointer that was passed in.
	threadInputs[0].threadOutputs -> foundIndexes = foundIndexes;

	// Start the search.
	recursiveSearch((void *) threadInputs);

	printf("Max = %d\n", threadOutputs -> foundMax);
	foundCount = threadOutputs -> foundCount;

	// Free the End!
	free(threadHandles);
    free(threadInputs);
	free(threadOutputs);

	//printf("No errors so far.\n");

	// Return -1 if not all 3 keys were found.
	int retval = (foundCount == 3) ? 0 : -1;
	return retval;
}


void *recursiveSearch(void *threadArgs)
{
	// Unpack the parameters.
	ThreadInputs *threadInputs = (ThreadInputs *) threadArgs;
	ThreadOutputs *threadOutputs = threadInputs -> threadOutputs;
	int* arr = threadInputs -> arr;
	int size = threadInputs -> size;
	int startIndex = threadInputs -> startIndex;
	int stopIndex = threadInputs -> stopIndex;
	int key = threadInputs -> key;
	int elementsToSearch = threadInputs -> elementsToSearch;
	size_t i, j;

	printf("I am pthread %d, searching from %d to %d.\n", (int) pthread_self(), startIndex, stopIndex);

	// If we have a small enough list, do not spawn more threads. Just search.
	if (elementsToSearch <= MIN_LIST_SIZE)
	{
		atomicSearch(threadArgs);
	}
	// Otherwise, spawn two more threads.
	else
	{
		//printf("PREPARING TO SPAWN 2 MORE THREADS.\n");
		// Prepare to spawn two more threads.
		pthread_t *newThreadHandles = calloc(2, sizeof (pthread_t));  // Array of thread handles for each thread.
		ThreadInputs *newThreadInputs = calloc(2, sizeof (ThreadInputs));  // Array of thread arguments for each thread.
		ThreadOutputs *newThreadOutputs = calloc(2, sizeof (ThreadOutputs));  // Array of thread outputs for each thread.
		int newThreadElements[2] = {elementsToSearch / 2, elementsToSearch / 2 + elementsToSearch % 2};
		int newThreadStartIndices[2] = {startIndex, startIndex + newThreadElements[0]};
		int newThreadStopIndices[2] = {newThreadStartIndices[1] - 1, startIndex + elementsToSearch - 1};
		int foundCount = 0;
		int max = arr[startIndex];
		int err;

		// Check for error with calloc.
		if (newThreadHandles == NULL || threadInputs == NULL || threadOutputs == NULL)
		{
			printf("Error allocating memory. Exiting...\n");
			exit(-1);
		}

		//printf("PREPARING TO FILL NEW THREAD ARGS.\n");

		// Fill the Thread args.
		for (i = 0; i < 2; i++)
		{
			newThreadInputs[i].threadOutputs = &(newThreadOutputs[i]);
			newThreadInputs[i].arr = arr;
			newThreadInputs[i].size = size;
			newThreadInputs[i].startIndex = newThreadStartIndices[i];
			newThreadInputs[i].stopIndex = newThreadStopIndices[i];
			newThreadInputs[i].key = key;
			newThreadInputs[i].elementsToSearch = newThreadElements[i];

			// Allocate output space.
			newThreadInputs[i].threadOutputs -> foundIndexes = calloc(3, sizeof(int));
			// Check for error with calloc.
			if (newThreadInputs[i].threadOutputs -> foundIndexes == NULL)
			{
				printf("Error allocating memory. Exiting...\n");
				exit(-1);
			}
		}

		//printf("ABOUT TO SPIN UP 2 MORE THREADS.\n");

		// Spin up threads.
		for (i = 0; i < 2; i++)
		{
			err = pthread_create(&newThreadHandles[i], NULL, (void * (*)(void *)) recursiveSearch, (void *) &newThreadInputs[i]);
			if (err)
			{
				printf("Error creating thread #%zu.\n", i);
				//exit(-1);
			}
		}

		// Join threads in order and collect the results.
		for (i = 0; i < 2; i++)
		{
			ThreadOutputs *currentOutput;
			err = pthread_join(newThreadHandles[i], (void **) &currentOutput);
			if (err)
			{
				printf("Error joining thread #%zu.\n", i);
				//exit(-1);
			}
			else 
			{
				for (j = 0; j < currentOutput -> foundCount; j++)
				{
					threadOutputs -> foundIndexes[foundCount++] = currentOutput -> foundIndexes[j];
					threadOutputs -> foundCount = foundCount;
				}

				if (currentOutput -> foundMax > max)
				{
					max = currentOutput -> foundMax;
					threadOutputs -> foundMax = currentOutput -> foundMax;
				}
				else
				{
					threadOutputs -> foundMax = max;
				}
			}
		}

		// Free the End!
		free(newThreadHandles);
		free(newThreadInputs);

		for (i = 0; i < 2; i++)
		{
			free(newThreadOutputs[i].foundIndexes);
		}
		free(newThreadOutputs);
	}

	if (elementsToSearch == size)  // This is the main thread.
	{
		return NULL;
	}
	else
	{
		pthread_exit((void *) threadOutputs);
	}

	return NULL;  // To satisfy compiler, because GCC is a stinky poohead.
}


void *atomicSearch(void *threadArgs)
{
	// Unpack the parameters.
	ThreadInputs *threadInputs = (ThreadInputs *) threadArgs;
	ThreadOutputs *threadOutputs = threadInputs -> threadOutputs;
	int* arr = threadInputs -> arr;
	int size = threadInputs -> size;
	int startIndex = threadInputs -> startIndex;
	int stopIndex = threadInputs -> stopIndex;
	int key = threadInputs -> key;

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

	return NULL;  // To satisfy compiler, because GCC is a stinky poohead.
}


int main(int argc, char* argv[])
{
	// Error check on input, the length of the list.
    if (argc != 2)
    {
        printf("Invalid arguments: 1 expected, %d provided. \nRequires one integer: the length of the list to search.\n", argc - 1);
        return -1;
    }
    int listSize = atoi(argv[1]);

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

	int result = search(arr, listSize, listSize, key, foundIndexes);
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
