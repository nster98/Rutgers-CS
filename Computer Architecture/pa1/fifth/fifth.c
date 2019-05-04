#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isVowel(char c)
{
    if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u')
    {
        return 1;
    }
    return 0;
}

int main(int argc, char* argv[])
{
    int num = 0;

    for (int i = 1; i < argc; i++)
    {
        num = strlen(argv[i]);

        for (int j = 0; j < num; j++)
        {
            if (isVowel(argv[i][j]) == 1)
                printf("%c", argv[i][j]);
        }
    }
    printf("\n");
    return 0;
}
