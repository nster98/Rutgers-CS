/* Nathaniel Glikman
 * nmg116
 *
 * Asst2: Spooky Searching
 * Due: Nov 22nd
 *
 * searchtest.c
 */

#include "multitest.h"

// Create unique random array
int* createRandomArray(int* arr, int size)
{
	// Generate a sequential list of numbers
	for (int i = 0; i < size; i++)
	{
		arr[i] = i;
	}
	
	// Pick 2 random indeces and swap those numbers, do that <size> times
	for (int i = 0; i < size*3/4; i++)
	{
		int randIndex1 = rand() % size;
		int randIndex2 = rand() % size;

		int temp = arr[randIndex1];
		arr[randIndex1] = arr[randIndex2];
		arr[randIndex2] = temp;
	}

	return arr;
}

// Takes the previously searched index and swaps it with a randomly generated index with the
// intention of searching for that value again.
//
// (No need to worry about the value changing since we know the value were searching for, and we know we just swapped that value)
int* genNewIndex(int* arr, int size, int indexPrevSearched)
{
	int randIndex = rand() % size;

	if (randIndex >= size || indexPrevSearched >= size)
		printf("Index out of bounds");

	int temp = arr[randIndex];
	arr[randIndex] = arr[indexPrevSearched];
	arr[indexPrevSearched] = temp;

	return arr;
}

int main(int argc, char* argv[])
{
	//Get a seed for our randomizer
	srand(time(0));
	
	// Initialize array size and make array
	int arraySize = 20000;
	int* array = (int*) malloc(sizeof(int) * arraySize);

	array = createRandomArray(array, arraySize);

	// Search for a random value that will stay the same
	//
	// The function search() will take the key (Value were searching for)
	// and depending on how it was compiled will use the process version
	// or the thread version. This means that multisearch.h will make
	// search() our dummy function
	//
	// Found index should come back differently based on how it was compiled
	// Processes:
	// 		Since we need to only do it in 250 sized batches to get the index from exit(),
	// 		we need to multiply the index by the number of the process, however the hell were
	// 		gonna get that is to be determined
	//
	// Threads:
	// 		Should be more direct to get the index
	// 		Use pthread_exit() to get back the exit in a pointer
	
	// Generate key
	int key = rand() % arraySize;

	int factor = 80;		// Amount of processes / threads to make
	int timesToRun = 25;	// Amount of times to switch the key and search
	int ulimit = 2000;		// ulimit set by the ilabs

	if (timesToRun * factor > ulimit)
	{
		printf("Went over ulimit, stopping program\n");
		return 0;
	}

	int foundIndex = 0;
	
	struct timeval start, end;

	// Set up values for getting time
	float avgTimeToRun = 0;
	float minTime = FLT_MAX;
	float maxTime = FLT_MIN;

	float* standardTime = malloc(sizeof(float)*timesToRun);

	for (int i = 0; i < timesToRun; i++)
	{
		gettimeofday(&start, NULL);	
	
		// Generate a new index for the key
		array = genNewIndex(array, arraySize, foundIndex);

		// Search using the search function we compiled with
		foundIndex = search(array, arraySize, factor, key);

		// Only returns -2 if the process search was given too many numbers to search, so we need to exit
		if (foundIndex == -2)
			return 0;

		if (foundIndex > 0 && foundIndex < arraySize)
			printf("Runthrough: %d\tKey: %d\tarray[foundIndex]: %d\tIndex: %d\n", i+1, key, array[foundIndex], foundIndex);
		
		gettimeofday(&end, NULL);
		
		float timeTook = (float)((end.tv_sec - start.tv_sec) * 10000000 + (end.tv_usec - start.tv_usec) * 0.000001);

		if (timeTook < minTime)
			minTime = timeTook;

		if (timeTook > maxTime)
			maxTime = timeTook;
		
		standardTime[i] = timeTook;
		avgTimeToRun += timeTook;
	}
	
	avgTimeToRun /= timesToRun;


	// Calculate standard deviation
	float temp;	
	for(int i = 0; i < timesToRun;i++)
	{
		temp = standardTime[i] - avgTimeToRun;
		temp = temp * temp;
		standardTime[i] = temp;
	}
	temp = 0;	
	for(int i = 0; i < timesToRun;i++)
	{
		temp += standardTime[i];
	}
	float standardDeviation = temp/timesToRun;
	standardDeviation = sqrt(standardDeviation);

	// Print values
	printf("=================================================================\n");
	printf("Mininum time to run: %.8f µs\n", minTime);
	printf("Maximum time to run: %.8f µs\n", maxTime);
	printf("Average time to run: %.8f µs\n", avgTimeToRun);
	printf("Standard Deviation: %.8f µs\n\n", standardDeviation);

	return 0;	
}
