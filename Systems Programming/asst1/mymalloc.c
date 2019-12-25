/* Nathan Glikman		Andrew Cater
 * nmg116				ajc398
 * 177003417			179009557
 *
 * ++Malloc
 * Due: October 15th 2019
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "mymalloc.h"

node* list = (void*) block; // Main Memory

void* mymalloc(size_t sizeWanted, char* file, int line)
{
	node* curPtr;
	void* retPtr;

	// First check to see if it's the first call to malloc, so we can initialize the memory
	if (list->size == 0)
	{
		initializeMemory(sizeWanted);
		return list->next;
	}

	// Start at the beginning of our block of memory
	curPtr = list;

	//counts nodes in the list
	int counter = 1;

	// Check free nodes until finding one that fits size wanted
	while ((curPtr->isFree != 1 || curPtr->size < sizeWanted) && curPtr->next != NULL)
	{
		curPtr = curPtr->next;
		counter++;
	}
	// 3 CASES
	// 1. WE HAVE EXACTLY THE AMOUNT OF SPACE THAT THE USER WANTED
	// 2. WE HAVE MORE THAN WHAT THE USER WANTED (SPLIT MEMORY HERE)
	// 3. ERROR: DID NOT HAVE ENOUGH MEMORY
	// Return the retPtr that points to the block we wanted
	if (list->size == sizeWanted)
	{
		curPtr->isFree = 0;
		retPtr = (void*)(curPtr++);
		return retPtr;
	}
	else if (list->size > sizeWanted + sizeof(node))
	{
		splitMemory(list, sizeWanted);
		node* yeetPtr = list;
		while(yeetPtr->next != NULL)
		{
			yeetPtr= yeetPtr->next;
		}
		retPtr = (void*)yeetPtr;
		return retPtr;
	}
	else if (list->size < sizeWanted)
	{
		retPtr = NULL;
		printf("Error: Size requested was too large. Called malloc on line %d in %s\n", line, file);
		return retPtr;
	}
}

void myfree(void* ptr, char* file, int line)
{
	node* curPtr = ptr;

	// Check to see if we can actually free the memory
	if (ptr <= (void*)(block + BLOCK_SIZE) && ptr >= (void*)(block)) // Is it in the bounds?
	{
		if (curPtr->isFree == 0)
		{
			curPtr->isFree = 1;
			mergeMemory();
		}
		else 
		{
			printf("Error: Memory has already been freed. Called free on line %d in %s\n", line, file);
		}
	}
	else
	{
		printf("Error: Pointer asked for is not valid. Called free on line %d in %s\n", line, file);
	}

}

// Called if the memory is empty
void initializeMemory(size_t size)
{
	list->size = BLOCK_SIZE;
	list->isFree = 1;
	list->next = NULL;
	splitMemory(list,size);
	
}

// Splits memory into a new node that points to the node with the extra memory
void splitMemory(node* passedNode, size_t size)
{
	struct node* curPtr = passedNode;
	while(curPtr->next!=NULL)
	{
		curPtr = curPtr->next;
	}	
	struct node* tempNode = ((void*)curPtr + size + sizeof(struct node)); 
	tempNode->size = size;
	tempNode->isFree = 0;
	tempNode->next = curPtr->next;
	curPtr->next = tempNode;
	
	passedNode->size = passedNode->size - size - sizeof(struct node);
	passedNode->isFree = 1;
}

// Merges memory for every free call so we don't have empty space;
void mergeMemory()
{
	node* curPtr;
	node* mainPtr;
	mainPtr = list;
	curPtr = list;
	while (curPtr->next != NULL)
	{
		if (curPtr->next->isFree == 1) 
		{
			mainPtr->size = mainPtr->size + curPtr->next->size + sizeof(node);
			curPtr->next = curPtr->next->next;
			break;
		}
		curPtr = curPtr->next;
	}
	if(mainPtr->next == NULL)
	{
	}
}
