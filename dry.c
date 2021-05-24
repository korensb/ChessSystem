#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* foo(char* str, int* x) {
	if (str == NULL)
	{
		return 0;
	}
	if (x == NULL)
	{
		return 0;
	}
	char* str2;
	int i;
	x = strlen(str);
	str2 = malloc(sizeof(int)*(*x));
	if (str2 == NULL)
	{
		return 0;
	}
	for (i = 0; i < *x; i++)
		str2[i] = str[*x - i - 1];
	if (*x % 2 == 0) {
		printf("%s", str);
	}
	if (*x % 2 != 0)
	{
		printf("%s", str2);
	}
	return str2;
}