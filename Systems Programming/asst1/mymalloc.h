/* Nathan Glikman		Andrew Cater
 * Header File
 *
 */

#ifndef mymalloc_h
#define mymalloc_h

#define malloc(x) mymalloc(x, __FILE__, __LINE__)
#define free(x) myfree(x, __FILE__, __LINE__)
#include <stdio.h>

#define BLOCK_SIZE 4096

static char block[BLOCK_SIZE];

typedef struct node
{
	size_t size;
	int isFree;
	struct node *next;

} node;

void* mymalloc(size_t, char*, int);
void myfree(void* ptr, char*, int);
void initializeMemory();
void splitMemory(node* node, size_t);
void mergeMemory();


#endif
