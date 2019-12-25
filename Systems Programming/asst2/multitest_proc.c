/* Nathaniel Glikman
 * nmg116
 *
 * Asst2: Spooky Searching
 * Due: Nov 22nd
 *
 * multitest_proc.c
 */

#include "multitest.h"

int nameFlag = 0;

// Search through the array for the key in amount sized
// batches and return back the index using exit() with the value
// times the child ID
int search(int* arr, int size, int factor, int key)
{		
	int retIndex = -1;
	int amount = size / factor;
	int status;
	int num = 0;
	unsigned char exit_code;
	pid_t pid[factor];
	int sectionOfChild[factor];

	if (amount > 250 && nameFlag == 0)
	{
		printf("Processes can't search more than 250 numbers, exiting...\n");
		nameFlag = 1;
		return -2;
	}

	// Fork time
	for (int i = 0; i < size; i += amount)
	{
		pid[i / amount] = fork();
		num = i;

		if (pid[num / amount] < 0) // There was an error
		{
			fprintf(stderr, "Failed to fork\n");
			exit(-1);
		}

		if (pid[num / amount] == 0) // We are in child now
		{
			int foundFlag = 0;

			int end = (num + amount < size) ? num + amount : size;

			for (int j = num; j < end; j++)
			{
				if (arr[j] == key)
				{
					foundFlag = 1;
					exit(j % amount);
				}
			}
			if (foundFlag == 0)
			{
				exit(-1);
			}
		}
		else // We are in parent now
		{
			if (nameFlag == 0)
			{
				printf("Running Mode: Process\n");
				nameFlag = 1;
			}
			
		}
	}

	// For each child we have stored the PIDs of in an array, wait on each and get
	// the exit code, break when we find the correct one and return it
	int k;
	for (k = 0; k < amount; k++)
	{
		waitpid(pid[k], &status, 0);
		sectionOfChild[k] = k * amount;

		if (WIFEXITED(status))
		{
			exit_code = WEXITSTATUS(status);

			if (exit_code != 255)
				break;
		}
	}

	if (exit_code < 255)
	{
		return exit_code + sectionOfChild[k];
	}
	else
		return -1;
}
