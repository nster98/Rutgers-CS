// Nathan Glikman
// Gregory Giovannini
// Problem 2 Question 2
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

int search(int* arr, int size, int factor, int key, int* foundIndexes);
int atomicSearch(int* arr, int size, int startIndex, int stopIndex, int key, int* foundIndexes, int* foundMax);

#define KEY -50
#define ULIMIT 31830
#define PROC_LIST_SIZE 250

int search(int* arr, int size, int factor, int key, int* foundIndexes)
{	
	int foundCount = 0, max = arr[0];
	int searchResult;
	int subArrayLower, subArrayUpper;
	int* atomicFoundIndexes = (int*) malloc(3 * sizeof(int));
	int* atomicFoundMax = (int *) malloc(sizeof(int));
	size_t i, j;
	pid_t pidResult, parentOfAll;

	int pipefd[2];
	if (pipe(pipefd) == -1)
	{
		printf("Error: could not create pipe.\n");
		exit(1);
	}

	printf("I am process %d, my parent is %d\n", getpid(), getppid());
	parentOfAll = getpid(); 

	// First, spawn all processes.
	// Each process can start searching as soon as it is created.
	// However, if it has a child, the process needs to wait on that child
	// before it can pipe its information up the chain of processes.

	// Spawn <factor - 1> number of processes.
	for (i = 1; i <= factor; i++)
	{
		// Determine the start and end of the subarray to be searched.
		subArrayLower = (i - 1) * PROC_LIST_SIZE;
		subArrayUpper = (i) * PROC_LIST_SIZE - 1;

		// Spawn a child process.
		pidResult = fork();

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
			// See if this will be the last process.
			if (i == factor)
			{
				// Pipe the starting data to the parent.
				// First send max.
				write(pipefd[1], &max, sizeof(int));
				// Next send number of found indexes (should be 0).
				write(pipefd[1], &foundCount, sizeof(int));
				// Now, pipe each found index (should be none).
				for (j = 0; j < foundCount; j++)
				{
					write(pipefd[1], &(foundIndexes[j]), sizeof(int));
				}
			}
		}
		// If this process is the parent of the current fork:
		else if (pidResult > 0)
		{
			// Just search, then break.
			searchResult = atomicSearch(arr, size, subArrayLower, subArrayUpper, key, atomicFoundIndexes, atomicFoundMax);

			for (j = 0; j < searchResult; j++)
			{
				foundIndexes[j] = atomicFoundIndexes[j];
				foundCount++;
			}

			// Wait for the child.
			waitpid(pidResult, NULL, 0);

			// Read the data from the child's search, and compare it to this process's search.
			int readFoundMax, readFoundCount, readFoundIndex;
			// First read max.
			read(pipefd[0], &readFoundMax, sizeof(int));
			// Next read number of found indexes.
			read(pipefd[0], &readFoundCount, sizeof(int));
			// Now, read each found index.
			for (j = 0; j < readFoundCount; j++)
			{
				read(pipefd[0], &readFoundIndex, sizeof(int));
				foundIndexes[foundCount++] = readFoundIndex;
				//printf("Reading index: %d\n", readFoundIndex);
			}

			// Compare.
			if (*atomicFoundMax >= readFoundMax)
			{
				max = *atomicFoundMax;
			}

			// If this process is NOT the starting process, pipe.
			if (!(getpid() == parentOfAll))
			{
				// Pipe the data from this search to the parent of this process.
				// First send max.
				write(pipefd[1], &max, sizeof(int));
				// Next send number of found indexes.
				write(pipefd[1], &foundCount, sizeof(int));
				// Now, pipe each found index.
				for (j = 0; j < foundCount; j++)
				{
					write(pipefd[1], &(foundIndexes[j]), sizeof(int));
					//printf("Writing index: %d\n", foundIndexes[j]);
				}
			}

			break;
		}
	}

	free(atomicFoundIndexes);
	free(atomicFoundMax);

	// If this process is the starting process, return.
	if (getpid() == parentOfAll)
	{
		printf("Max = %d\n", max);

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


int atomicSearch(int* arr, int size, int startIndex, int stopIndex, int key, int* foundIndexes, int* foundMax)
{
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
			foundIndexes[foundCount++] = i;
			printf("I am process %d and I found the key at %d\n", getpid(), foundIndexes[foundCount - 1]);
		}
		// All found check.
		if (foundCount == 3)
		{
			break;
		}
	}

	// Save the maximum.
	*foundMax = max;

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
