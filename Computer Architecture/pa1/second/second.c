#include <stdio.h>
#include <stdlib.h>

/* Node Struct */
typedef struct node
{
	int data;
	struct node* next;
} node;

/* Make a new node */
node* makeNewNode(int data, node* front)
{
	struct node* newNode = malloc(sizeof(node));
	newNode->data = data;
	newNode->next = front;
	return newNode;
}

/* Delete specific node */
node* deleteNode(int data, node* front)
{
	if (front == NULL)
		return front;

	if (front->data == data)
	{
		if (front->next == NULL)
		{
			front = front->next;
			return front;
		}
		else
		{
			front = front->next;
			return front;
		}

		front = front->next;

	}

	struct node* ptr = front->next;
	struct node* prev = front;

	while (ptr != NULL && ptr->data != data)
	{
		prev = ptr;
		ptr = ptr->next;
	}

	if (ptr == NULL) return front;

	prev->next = ptr->next;

	free(ptr);

	return front;
}

/* Gets the size of the Linked List */
int getSize(node* front)
{
	int size = 0;
	if (front == NULL)
		return size;

	struct node* ptr = front;

	while (ptr != NULL)
	{
		size++;
		ptr = ptr->next;
	}

	free(ptr);

	return size;
}
void freeLinkedList(node* front)
{
	struct node* prev;

	while (front != NULL)
	{
		prev = front;
		front = front->next;
		free(prev);
	}
}

/* Sorts the Linked List */
node* sort(node* front)
{
	struct node* ptr;
	struct node* lessPtr;
	struct node* stayPtr;

	int temp;
	stayPtr = front;

	while (stayPtr != NULL)
	{
		ptr = stayPtr->next;
		lessPtr = stayPtr;

		while (ptr != NULL)
		{
			if (ptr->data < lessPtr->data)
			{
				lessPtr = ptr;
			}

			ptr = ptr->next;
		}

		temp = stayPtr->data;
		stayPtr->data = lessPtr->data;
		lessPtr->data = temp;

		stayPtr = stayPtr->next;
	}

	return front;
}

/* TODO: MUST PRINT OUT ONLY UNIQUE VALUES BUT THE SIZE IS STILL THE RIGHT AMOUNT */
int main(int argc, char* argv[])
{
	struct node* front = NULL;
	struct node* ptr = NULL;

	int size, num;
	char operation;

	FILE *file;
	file = fopen(argv[1], "r");

	if (file == NULL)
	{
		printf("error\n");
		return 0;
	}

	while (fscanf(file, "%c\t%d\n", &operation, &num) != EOF)
	{
		if (operation == 'i')
		{
			front = makeNewNode(num, front);
		}
		else if (operation == 'd')
		{
			front = deleteNode(num, front);
		}
	}

	ptr = front;
	size = getSize(ptr);

	if (size == 0)
	{
		printf("%d\n", size);
		return 0;
	}

	printf("%d\n", size);

	front = sort(front);

	ptr = front;

	while (ptr != NULL)
	{
		int prevNum;

		if (prevNum != ptr->data)
			printf("%d\t", ptr->data);

		prevNum = ptr->data;
		ptr = ptr->next;
	}
	printf("\n");

	fclose(file);
	freeLinkedList(front);

	return 0;
}

