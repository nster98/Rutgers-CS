// Nathan Glikman
// Gregory Giovannini
// Problem 2 Question 4 - Variation on the Problem
// search_variation.c

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

int search(int* arr, int size, int factor, int key, int* foundIndexes);
int recursiveSearch(int* arr, int size, int factor, int key, int* foundIndexes,
					int procsToSpawn, int procLayer, int procSiblings,
					int siblingNum, pid_t parentOfAll);
int atomicSearch(int* arr, int size, int startIndex, int stopIndex, int key, int* foundIndexes);

#define KEY -50
#define ULIMIT 31830
#define PROC_LIST_SIZE 1000

typedef struct {
	pid_t pid;
	int foundCount;
	int foundIndex1;
	int foundIndex2;
	int foundIndex3;
} pipeArgs;

int search(int* arr, int size, int factor, int key, int* foundIndexes)
{
	int result = recursiveSearch(arr, size, factor, key, foundIndexes, factor - 1,
								 1, 0, 0, getpid());

	return result;
}


int recursiveSearch(int* arr, int size, int factor, int key, int* foundIndexes,
					int procsToSpawn, int procsSpawned, int procSiblings,
					int siblingNum, pid_t parentOfAll)
{
	int foundCount = 0;
	int searchResult;
	int subArrayLower, subArrayUpper;
	int* atomicFoundIndexes = (int*) malloc(3 * sizeof(int));
	int* atomicFoundMax = (int *) malloc(sizeof(int));
	size_t i, j;
	pid_t pidResult;
	pid_t forkedProcs[procsToSpawn];
	int forkedProcCount = 0;

	int pipefd[2];
	if (pipe(pipefd) == -1)
	{
		printf("Error: could not create pipe.\n");
		exit(1);
	}
	// printf("NEW PIPE WITH FD %d %d\n", pipefd[0], pipefd[1]);
	// printf("PARENT PIPE WITH FD %d %d\n", parentPipefd[0], parentPipefd[1]);

	printf("I am process %d, my parent is %d\n", getpid(), getppid());
	//printf("I am process %d, my parent is %d, ID %d\n", getpid(), getppid(), procsSpawned); 

	// Spawn all child processes.
	for (i = 0; procsToSpawn > 0; i++)
	{
		// Spawn a child process.
		pidResult = fork();
		procsToSpawn--;
		procsSpawned++;

		//procsSpawned += (procSiblings - i + 1);

		// On error, return.
		if (pidResult < 0)
		{
			printf("Error: could not fork a new process.\n");
			exit(1);
		}
		// If this process is the child process of the current fork:
		else if (pidResult == 0)
		{
			printf("I am process %d, my parent is %d\n", getpid(), getppid());
			// Search, pipe, then exit.

			// Determine the start and end of the subarray to be searched.
			subArrayLower = (procsSpawned - 1) * PROC_LIST_SIZE;
			subArrayUpper = (procsSpawned) * PROC_LIST_SIZE - 1;

			searchResult = atomicSearch(arr, size, subArrayLower, subArrayUpper, key, atomicFoundIndexes);

			for (j = 0; j < searchResult; j++)
			{
				foundIndexes[j] = atomicFoundIndexes[j];
				foundCount++;
			}

			// Pipe the data from this search to the parent of this process.
			pipeArgs writePipeArgs;
			// Send number of found indexes.
			writePipeArgs.foundCount = foundCount;
			// Now, pipe each found index.
			writePipeArgs.foundIndex1 = foundIndexes[0];
			writePipeArgs.foundIndex2 = foundIndexes[1];
			writePipeArgs.foundIndex3 = foundIndexes[2];
			write(pipefd[1], &writePipeArgs, sizeof(pipeArgs));

			exit(0);
		}
		// If this process is the parent of the current fork:
		else if (pidResult > 0)
		{
			forkedProcs[forkedProcCount++] = pidResult;

			// Continue to keep forking.
			continue;
		}
	}

	// Here, we should be the parent of all.

	// Determine the start and end of the subarray to be searched.
	subArrayLower = (1 - 1) * PROC_LIST_SIZE;
	subArrayUpper = (1) * PROC_LIST_SIZE - 1;
	
	// Do the parent's search.
	searchResult = atomicSearch(arr, size, subArrayLower, subArrayUpper, key, atomicFoundIndexes);

	for (j = 0; j < searchResult; j++)
	{
		foundIndexes[j] = atomicFoundIndexes[j];
		foundCount++;
	}

	// Wait for the children.
	for (j = 0; j < forkedProcCount; j++)
	{
		waitpid(forkedProcs[j], NULL, 0);
	}

	// Read the data from each child's search (if any), and compare it to this process's search.
	for (i = 0; i < forkedProcCount; i++)
	{
		//printf("READING, ID %d\n", originalProcsSpawned);
		// Read the data.
		pipeArgs readPipeArgs;
		read(pipefd[0], &readPipeArgs, sizeof(pipeArgs));
		//printf("DONE READING, ID %d\n", originalProcsSpawned);

		int readFoundCount;
		int readFoundIndexes[3];
		// Read number of found indexes.
		readFoundCount = readPipeArgs.foundCount;
		// Now, read each found index.
		readFoundIndexes[0] = readPipeArgs.foundIndex1;
		readFoundIndexes[1] = readPipeArgs.foundIndex2;
		readFoundIndexes[2] = readPipeArgs.foundIndex3;
		for (j = 0; j < readFoundCount; j++)
		{
			foundIndexes[foundCount++] = readFoundIndexes[j];
		}
	}

	free(atomicFoundIndexes);
	free(atomicFoundMax);

	// If this process is the starting process, return.
	if (getpid() == parentOfAll)
	{
		// Return -1 if not all 3 keys were found.
		int retVal = (foundCount == 3) ? 0 : -1;
		return retVal;
	}
	// If this process is any other process, simply exit.
	else
	{
		exit(0);
	}
}


int atomicSearch(int* arr, int size, int startIndex, int stopIndex, int key, int* foundIndexes)
{
	int foundCount = 0;
	size_t i;

	for (i = startIndex; i < size && i < stopIndex; i++)
	{
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

	// Return number of keys found.
	return foundCount;
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

	// Set the keys at each point in the array.
	int key = KEY;
	arr[LIST_SIZE / 4] = key;
	arr[LIST_SIZE / 2] = key;
	arr[(3 * LIST_SIZE) / 4] = key;

	// Calculate the amount of processes to make here.
	int factor = (LIST_SIZE % PROC_LIST_SIZE == 0) ? LIST_SIZE / PROC_LIST_SIZE : LIST_SIZE / PROC_LIST_SIZE + 1;
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
		printf("Key found at index: %d\n", foundIndexes[i]);
	}

	// Stop timing here.
	gettimeofday(&end, NULL);
    timersub(&end,   &start, &diff);

	printf("Search took %'8.3f ms.\n", diff.tv_sec*1000.0 + diff.tv_usec/1000.0);
}
