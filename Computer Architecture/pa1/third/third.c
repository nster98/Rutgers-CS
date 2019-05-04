#include <stdio.h>
#include <stdlib.h>

/* Struct for node */
typedef struct node
{
	int data;
	struct node* next;
} node;

/* Struct for hash table */
typedef struct hashtable
{
	int size;
	struct node** row;
} hashtable;

/* Make the main table */
hashtable* makeTable(int size)
{
	struct hashtable *table = (struct hashtable*) malloc(sizeof(struct hashtable));

	table->size = size;
	table->row = malloc(sizeof(struct node*) * size);

	for (int i = 0; i < size; i++)
		table->row[i] = NULL;

	return table;

}

int getKey(int num, int size)
{
	int ans = num % size;
	return ans;
}

/*
	Search for num in table
	Return 1 if found
	Return 0 if not found
*/
int search(struct hashtable* table, int num)
{
	int key;

	if (num < 0)
	{
		key = getKey(num * -1, table->size);
	} else {
		key = getKey(num, table->size);
	}

	if (table->row[key] == NULL)
	{
		return 0;
	}

	if (table->row[key]->data == num)
	{
		return 1;
	}

	struct node* ptr = table->row[key];

	while (ptr->next != NULL)
	{
		if (ptr->next->data == num)
		{
			return 1;
		}

		ptr = ptr->next;
	}

	return 0;
}

/*
	Insert num into the hash table
	Return 1 if successfully added
	Return 0 if not added
*/
int insert(struct hashtable* table, struct node* node)
{
	int data = node->data;
	int key;

	if (data < 0)
	{
		key = getKey(data * -1, table->size);
	} else {
		key = getKey(data, table->size);
	}

	struct node* ptr;
	int sameKey = 0;

	if (table->row[key] == NULL)
	{
		table->row[key] = node;
		return 1;
	}

	if (table->row[key]->data == data)
	{
		return 2;
	}

	if (table->row[key] != NULL)
		sameKey = 1;

	ptr = table->row[key];

	while(ptr->next != NULL)
	{
		if (ptr->next->data == data)
		{
			return 2;
		}

		ptr = ptr->next;
	}

	ptr->next = node;

	if (sameKey == 1)
		return 2;

	return 1;
}

/* Create a new node */
node* createNode(int data, struct node* front)
{
	node* newNode = (node*) malloc(sizeof(node));
	newNode->data = data;
	newNode->next = front;
	front = newNode;

	return front;
}
void freeNode(struct node* front)
{
	struct node* prev;

	while (front != NULL)
	{
		prev = front;
		front = front->next;
		free(prev);
	}
}
void freeHashTable(struct hashtable* table)
{
	for (int i = 0; i < 10000; i++)
	{
		if (table->row[i] != NULL)
		{
			freeNode(table->row[i]);
		}
	}
	free(table->row);
	free(table);
}



int main(int argc, char* argv[])
{
	struct hashtable* table = makeTable(10000);

	FILE* file;

	file = fopen(argv[1], "r");
	if (file == NULL)
	{
		printf("error\n");
		return 0;
	}

	int num;
	char operation;
	int didInsert;
	int didSearch;
	int insertCount = 0, searchCount = 0;
	struct node* temp = NULL;

	while (fscanf(file, "%c\t%d\n", &operation, &num) != EOF)
	{
		if (operation == 'i')
		{
			temp = NULL;
			temp = createNode(num, temp);
			didInsert = insert(table, temp);

			if (didInsert == 2)
			{
				insertCount++;
			}
		}
		if (operation == 's')
		{
			didSearch = search(table, num);

			if (didSearch == 1)
				searchCount++;
		}
	}

	printf("%d\n%d\n", insertCount, searchCount);

	fclose(file);
	freeHashTable(table);

	return 0;
}
