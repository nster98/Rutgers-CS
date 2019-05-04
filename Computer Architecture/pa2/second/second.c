#include <stdio.h>
#include <stdlib.h>

int get(int x, int n)
{
	return (x >> n) & 1U;
}
int getNumberOfBits(unsigned short num)
{
	int count = 0;
	while (num)
	{
		count++;
		num >>= 1;
	}
	return count;
}
void getNumberOfOneBits(unsigned short num)
{
	int oneBits = 0;
	for (int i = 0; i < getNumberOfBits(num); i++)
	{
		int bit = get(num, i);

		if (bit == 1)
			oneBits++;
	}
	if (oneBits % 2 == 0)
		printf("Even-Parity\t");
	else
		printf("Odd-Parity\t");

	return;
}
int getParity(unsigned short num)
{
	int pairs = 0;
	for (int i = 0; i < getNumberOfBits(num); i++)
	{
		int curr = get(num, i);
		int next = get(num, i + 1);

		if (next == 1 && curr == 1 && curr == next)
		{
			pairs++;
			i++;
		}
	}
	return pairs;
}

int main(int argc, char* argv[])
{
	unsigned short num = atoi(argv[1]);

	getNumberOfOneBits(num);
	printf("%d\n", getParity(num));

	return 0;
}
