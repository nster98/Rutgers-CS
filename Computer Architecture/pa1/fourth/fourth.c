#include <stdio.h>
#include <stdlib.h>

void print(int** matrix, int rows, int columns)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			if (j == columns - 1)
				printf("%d", matrix[i][j]);
			else
				printf("%d\t", matrix[i][j]);
		}
		printf("\n");
	}
}

int** multiply(int** matrix1, int** matrix2, int** result, int row1, int column1, int row2, int column2)
{
	int sum;

	for (int i = 0; i < row1; i++)
	{
		for (int j = 0; j < column2; j++)
		{
			sum = 0;

			for (int k = 0; k < column1; k++)
			{
				sum = sum + (matrix1[i][k] * matrix2[k][j]);
			}

			result[i][j] = sum;
		}
	}

	return result;
}

void freeMatrix(int** matrix, int row)
{
	for (int i = 0; i < row; i++)
	{
		free(matrix[i]);
	}
	free(matrix);
}

int main(int argc, char* argv[])
{
	FILE* file;
	file = fopen(argv[1], "r");

	if (file == NULL)
	{
		printf("error\n");
		return 0;
	}

	int row1, column1, row2, column2;

	//--------------------------
	// First Matrix Operations
	//--------------------------
	fscanf(file, "%d\t%d\n", &row1, &column1);

	int** matrix1 = (int**) malloc(row1 * sizeof(int*)); // MIGHT CAUSE PROBLEMS WITH MEMORY

	for (int i = 0; i < row1; i++)
		matrix1[i] = (int*) malloc(column1 * sizeof(int));

	int dataFromFile1;
	for (int i = 0; i < row1; i++)
	{
		for (int j = 0; j < column1; j++)
		{
			fscanf(file, "%d\t", &dataFromFile1);
			matrix1[i][j] = dataFromFile1;
		}
		fscanf(file, "\n");
	}

	//---------------------------
	// Second Matrix Operations
	//---------------------------
	fscanf(file, "%d\t%d\n", &row2, &column2);

	int dataFromFile2;
	int** matrix2 = (int**) malloc(row2 * sizeof(int*));

	for (int i = 0; i < row2; i++)
		matrix2[i] = (int*) malloc(column2 * sizeof(int));

	for (int i = 0; i < row2; i++)
	{
		for (int j = 0; j < column2; j++)
		{
			fscanf(file, "%d\t", &dataFromFile2);
			matrix2[i][j] = dataFromFile2;
		}
		fscanf(file, "\n");
	}

	if (column1 != row2)
	{
		printf("bad-matrices\n");
		return 0;
	}

	//--------------------------
	// Result Matrix Malloc
	// ** Size will be row1 x column2
	//--------------------------
	int** result = (int**) malloc(row1 * sizeof(int*));

	for (int i = 0; i < row1; i++)
		result[i] = (int*) malloc(column2 * sizeof(int*));

	// MULTIPLYING TIME
	result = multiply(matrix1, matrix2, result, row1, column1, row2, column2);

	print(result, row1, column2);

	freeMatrix(matrix1, row1);
	freeMatrix(matrix2, row2);
	freeMatrix(result, row1);

	fclose(file);

	return 0;
}
