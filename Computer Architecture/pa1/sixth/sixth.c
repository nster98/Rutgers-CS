#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
	int data;
	int level;
	struct node* left;
	struct node* right;

} node;

int insert(node* root, int data)
{
	int level = 1;

	struct node* ptr = root;
	struct node* prev = NULL;

	if (root->level == 0)
	{
		root->left = NULL;
		root->right = NULL;
		root->data = data;
		root->level = 1;

		return 1;
	}

	struct node* newNode = (node*) malloc(sizeof(node));

	newNode->left = NULL;
	newNode->right = NULL;
	newNode->data = data;
	newNode->level = 0;

	while (ptr != NULL)
	{
		if (ptr->data == newNode->data)
		{
			free(newNode);
			return 0; // Duplicate
		}

		if (ptr->data > newNode->data)
		{
			prev = ptr;
			ptr = ptr->left;
			level++;
		}
		else
		{
			prev = ptr;
			ptr = ptr->right;
			level++;
		}
	}

	newNode->level = level;

	if (prev->data > newNode->data)
	{
		prev->left = newNode;
		return newNode->level; // Return level TODO: PROBABLY DONT WANT THIS
	}
	else
	{
		prev->right = newNode;
		return newNode->level;
	}
}

void inOrderTraversal(node* root)
{
	if (root == NULL)
		return;

	inOrderTraversal(root->left);

	printf("%d\t", root->data);

	inOrderTraversal(root->right);
}

void freeTheTree(node* root)
{
	if (root == NULL)
		return;

	freeTheTree(root->left);
	freeTheTree(root->right);

	free(root);
}

int main(int argc, char* argv[])
{
	struct node* root = (node*) malloc(sizeof(node));
	root->level = 0;

	FILE* file = fopen(argv[1], "r");

	if (file == NULL)
	{
		printf("error\n");
		return 0;
	}

	char operation;
	int data;
	//int level = 0;

	while (fscanf(file, "%c\t%d\n", &operation, &data) != EOF)
	{
		if (operation == 'i')
		{
			insert(root, data);
		}
	}

	inOrderTraversal(root);

	printf("\n");

	freeTheTree(root);
	fclose(file);

	return 0;
}
