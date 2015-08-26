#include <stdio.h>

// raise x to the y-th power
int power(int x, int y)
{
	int i;
	int result = 1;

	for (i =0; i < y; i++) {
		result *= x;
	}

	return result;
}

// what if we wanted to write this after main?
int main()
{
	int result;

	result = power(3,4);
	printf("3 to the power of 4 is %i \n", result);
	return 0;
}