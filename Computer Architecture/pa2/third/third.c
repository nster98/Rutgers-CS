#include <stdio.h>
#include <stdlib.h>

int get(int x, int n)
{
	return (x >> n) & 1U;
}

void isPalindrome(unsigned short x)
{
	int left = 0;
	int right = sizeof(unsigned short) * 8 - 1;

	while (left < right)
	{
		if (get(x, left) != get(x, right))
		{
			printf("Not-Palindrome\n");
			return;
		}
		left++;
		right--;
	}
	printf("Is-Palindrome\n");
}

int main(int argc, char* argv[])
{
	unsigned short num = atoi(argv[1]);

	isPalindrome(num);

	return 0;
}
