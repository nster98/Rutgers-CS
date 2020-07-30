#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    // Error check on input, the length of the list to generate.
    if (argc != 2)
    {
        printf("Invalid arguments: 1 expected, %d provided. \nRequires a single number, the length of the number list to generate.\n", argc - 1);
        return -1;
    }
    int i, n, size = atoi(argv[1]);

    // Open the file and prepare to write to it.
    // Generate a list of n numbers between 1 and 100.
    FILE* file = fopen("arrayNums.txt", "w");
	for (i = 0; i < size; i++)
	{
        n = rand() % 100 + 1;
		fprintf(file, "%d\n", n);
	}

    // Close up shop.
    fclose(file);
    return 0;
}