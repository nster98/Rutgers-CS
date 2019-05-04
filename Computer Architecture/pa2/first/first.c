#include <stdio.h>
#include <stdlib.h>

int set(int x, int n, int v)
{
	return x = (x & (~(1 << n))) | (v << n);
}

int comp(int x, int n)
{
	return x ^= 1UL << n;
}

int get(int x, int n)
{
	return (x >> n) & 1U;
}

int main(int argc, char* argv[])
{
	int x;
	int firstArg;
	int secondArg;
	char method[256];

	FILE *file;
	file = fopen(argv[1], "r");

	fscanf(file, "%d\n", &x);

	while (fscanf(file, "%s\t%d\t%d\n", method, &firstArg, &secondArg) != EOF)
	{
		if (method[0] == 's')
		{
			x = set(x, firstArg, secondArg);
			printf("%d\n", x);
		}
		else if (method[0] == 'c')
		{
			x = comp(x, firstArg);
			printf("%d\n", x);
		}
		else if (method[0] == 'g')
		{
			printf("%d\n", get(x, firstArg));
		}
	}

	fclose(file);

	return 0;
}
