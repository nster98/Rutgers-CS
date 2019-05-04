#include <stdio.h>
#include <stdlib.h>

void selectionSwap(int *num1, int *num2)
{
    int temp = *num1;
    *num1 = *num2;
    *num2 = temp;
}

void printArray(int evenArr[], int oddArr[], int evenCount, int oddCount)
{
    for (int i = 0; i < evenCount; i++)
    {
        printf("%d\t", evenArr[i]);
    }
    for (int i = oddCount - 1; i >= 0; i--)
    {
        printf("%d\t", oddArr[i]);
    }
    printf("\n");
}

int main(int argc, char* argv[])
{
    int size;

    FILE* file;
    file = fopen(argv[1], "r");

    fscanf(file, "%d\n", &size);

    int arr[size];

    for (int i = 0; i < size; i++)
    {
        fscanf(file, "%d\t", &arr[i]);
    }
    /*int arr[6] = {25, 10, 1, 99, 4, 2};*/

    int evenCount = 0, oddCount = 0;

    /* Get the amount of evens and odds to put in a new array */
    /* TODO: Change 6 to the first line in the txt */
    for (int k = 0; k < size; k++)
    {
        if (arr[k] % 2 == 0)
        {
            /* If even */
            evenCount++;
        }
        else
            oddCount++;
    }

    /* Make new arrays for evens and odds */
    int evenArray[evenCount];
    int oddArray[oddCount];

    /* Add evens and odds to respective array */
    int evenIndex = 0, oddIndex = 0;
    for (int l = 0; l < size; l++)
    {
        if (arr[l] % 2 == 0)
        {
            evenArray[evenIndex] = arr[l];
            evenIndex++;
        }
        else
        {
            oddArray[oddIndex] = arr[l];
            oddIndex++;
        }
    }

    int evenMin, oddMin;

    /* Even sorting (INCREASING) */
    for (int i = 0; i < evenCount; i++)
    {
        evenMin = i;
        for (int j = i + 1; j < evenCount; j++)
        {
            if (evenArray[j] < evenArray[evenMin])
            {
                evenMin = j;
            }
        }
        selectionSwap(&evenArray[evenMin], &evenArray[i]);
    }

    /* Odd sorting (DECREASING) */
    for (int i = 0; i < oddCount; i++)
    {
        oddMin = i;
        for (int j = i + 1; j < oddCount; j++)
        {
            if (oddArray[j] < oddArray[oddMin])
            {
                oddMin = j;
            }
        }
        selectionSwap(&oddArray[oddMin], &oddArray[i]);
    }

    printArray(evenArray, oddArray, evenCount, oddCount);

    fclose(file);

    return 0;
}

