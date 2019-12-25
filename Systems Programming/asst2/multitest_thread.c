/*
 * Andrew Cater ajc398
 * Asst2: Spooky Searching
 * Due: Nov 22nd
 * multitest_thread.c
*/

#include "multitest.h"
#include <pthread.h>
int* array;
int goal;
int amount;
int arraysize;
int index;

int nameFlag = 0;

/*typedef struct threadbuild {
	int* search;
	pthread_t id;
	int start;
	int end;
	int goal;
}thread;
*/
void* find_value(void*);

int search(int* arr, int size, int factor, int key)
{
	//using same variables as proc to avoid confusion		
	amount = size/factor;
	array = arr;
	goal  = key;
	arraysize = size;
	pthread_t threads[factor];
	void* exitStatus = NULL;
	index = -1;
	//plan for rest of program
	/*
		1)	create function to run threads off off
		2)	allow function to read the array and send back and 
	exit code.
		3) check all exit codes for the target within the array
		4) create struct to send to thread function	
		5)	???
		6)	profit!
	*/

	if (nameFlag == 0)
	{
		printf("Running Mode: Threads\n");
		nameFlag = 1;
	}
	
	//this loop creates threads equal to the number sent into amount.
	for(int i = 0; i < factor; i++)
	{
		threads[i] = i;
		pthread_create(&(threads[i]),NULL, find_value, (void *)&i);
	}
	//all threads should be running now to join all threads and look for results
	for(int i = 0; i < factor; i++)
	{
		pthread_join(threads[i],exitStatus);
		//printf("Thread %d Exit Status %s\n",i,exitStatus);
		//printf("confustion");
		
	}
	return index;
}

/*void* find_value(void* _arg)
{
	thread *thisthread;
	thisthread = (thread *) _arg;
	for(int i = thisthread->start; i < thisthread->end; i++)
	{
		if(thisthread->goal == thisthread->search[i])
		{
			pthread_exit((void *)&i);
		}
	}
	pthread_exit((void *)-1);
}
*/
void* find_value(void* _arg)
{
	int id = *((int *)_arg);
	id--;
	//printf("Thread: %d \n", id);	
	int start = (amount * (id));
	int end  = (amount*((id)+1)<arraysize) ? amount*((id)+1) : arraysize;
	
//	printf("Start: %d \n", start);
	//printf("End: %d \n", end);		
	for(int i = start; i < end; i++)
	{
		if(array[i] == goal)
		{
			//printf("Looking for %d, found %d in thread %d at index %d\n\n",goal,array[i],id, i);
			index = i;
			pthread_exit((void *) (&i));			
		}
	}

	//printf("Found Nothing\n\n");
	pthread_exit(NULL);
}
